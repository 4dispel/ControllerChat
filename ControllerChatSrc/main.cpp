#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <libevdev/libevdev.h>
#include <fstream>
#include <cctype>
int main()
{
	using std::cout;
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::socket socket(io_context);
	boost::asio::ip::tcp::resolver resolver(io_context);
	auto endpoints = resolver.resolve("irc.twitch.tv", "6667");
	boost::asio::connect(socket, endpoints);

		
	std::string oauth_token, username, channel, a_text, b_text, x_text, y_text, up_text, down_text, left_text, right_text, l_text, zl_text, r_text, zr_text, 
		plus_text, minus_text, home_text, screenshot_text;
	bool swap;

	boost::program_options::options_description desc("Configuration options");
	desc.add_options()
		("oauth_token", boost::program_options::value<std::string>(&oauth_token), "password")
		("username", boost::program_options::value<std::string>(&username), "twitch username")	
		("channel", boost::program_options::value<std::string>(&channel), "channel to connect to")	
		("a_text", boost::program_options::value<std::string>(&a_text), "a")
		("b_text", boost::program_options::value<std::string>(&b_text), "b")
		("x_text", boost::program_options::value<std::string>(&x_text), "x")
		("y_text", boost::program_options::value<std::string>(&y_text), "y")
		("up_text", boost::program_options::value<std::string>(&up_text), "up")
		("down_text", boost::program_options::value<std::string>(&down_text), "down")
		("left_text", boost::program_options::value<std::string>(&left_text), "left")
		("right_text", boost::program_options::value<std::string>(&right_text), "right")
		("l_text", boost::program_options::value<std::string>(&l_text), "l")
		("zl_text", boost::program_options::value<std::string>(&zl_text), "zl")
		("r_text", boost::program_options::value<std::string>(&r_text), "r")
		("zr_text", boost::program_options::value<std::string>(&zr_text), "zr")
		("plus_text", boost::program_options::value<std::string>(&plus_text), "plus")
		("minus_text", boost::program_options::value<std::string>(&minus_text), "minus")
		("home_text", boost::program_options::value<std::string>(&home_text), "home")
		("screenshot_text", boost::program_options::value<std::string>(&screenshot_text), "screenshot")
		("swap", boost::program_options::value<bool>(&swap), "capitalize every seccond message");
	
	boost::program_options::variables_map vm;
	std::string configfile = "ControllerChat.cfg";
	std::ifstream file(configfile.c_str());
	if (file) {
		boost::program_options::store(boost::program_options::parse_config_file(file, desc), vm);
		boost::program_options::notify(vm);
	} else {
		std::cerr << "cannot open file" << std::endl;
	}	
	
	//cout << oauth_token << '\n' << username << '\n' << channel << '\n';
	//std::string oauth_token = "xh0u1hvypyfddbvsp0nc2q2p6ajch8";
	//std::string username = "mrredslimy";
	//std::string channel = "mrredslimy";
	socket.write_some(boost::asio::buffer("PASS oauth:" + oauth_token  + "\n"));
	socket.write_some(boost::asio::buffer("NICK " + username + "\n"));
	socket.write_some(boost::asio::buffer("JOIN #" + channel + "\n"));
	
	
	struct libevdev* dev = NULL;
	int fd, rc, event_index;
	for (int i = 0; i < 32; i++) {
		char filename[20];
		//cout << i;
		sprintf(filename, "/dev/input/event%d", i);
		fd = open(filename, O_RDONLY | O_NONBLOCK);
		if (fd < 0) {
			continue;
		}

		rc = libevdev_new_from_fd(fd, &dev);
		if (rc < 0) {
			close(fd);
			continue;
		}

		std::string name = libevdev_get_name(dev);
		//cout << ' ' << name << '\n';
		if (name == "Nintendo Switch Pro Controller") {
			cout << "found controller\n";
			event_index = i;
			break;
		}

		libevdev_free(dev);
		close(fd);
		dev = NULL;

	}
	if (dev = NULL) {
		std::cerr << "couldnt find controller" << std::endl;
		return 1;
	} else {
		libevdev_free(dev);
		close(fd);
		dev = NULL;	
		char filename[20];	
		sprintf(filename, "/dev/input/event%d", event_index);
		fd = open(filename, O_RDONLY|O_NONBLOCK);
		//cout << "fd: " << fd << '\n';
		dev = NULL;
		int err = libevdev_new_from_fd(fd, &dev);
		//cout << "device name: " << libevdev_get_name(dev) << '\n';
		//cout << "err: "<< err << '\n';
	
	
	
	}
	/*
	int fd = open("/dev/input/event13", O_RDONLY|O_NONBLOCK);
	cout << "fd: " << fd << '\n';
	struct libevdev* dev = NULL;
	int err = libevdev_new_from_fd(fd, &dev);
	cout << "device name: " << libevdev_get_name(dev) << '\n';
	cout << "err: "<< err << '\n';
	*/
	struct input_event ev;
	//cout << libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev) << '\n'; //Segmentation Fault in this line.
	libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
	std::string message = "";
	bool cycle = false;
	
	
	while (true) {
	while(libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev) == 0) {
		message = "";
		if (ev.type == EV_KEY) {
			/*if (ev.code == BTN_A) {
				cout << "a\n";
			} else if (ev.code == BTN_B) {
				cout << "b\n";
				break;
			} else {
				cout << ev.code << '\n';
			}*/
			if (ev.value == 1) {
			switch (ev.code){
				case BTN_B:
					cout << a_text << '\n';
					message = a_text;
					break;
				case BTN_A:
					cout << b_text << '\n';
					message = b_text;
					break;
				case 307:
					cout << x_text << '\n';
					message = x_text;
					break;
				case 308:
					cout << y_text << '\n';
					message = y_text;
					break;
				case 310:
					cout << l_text << '\n';
					message = l_text;
					break;
				case 312:
					cout << zl_text << '\n';
					message = zl_text;
					break;
				case 311:
					cout << r_text << '\n';
					message = r_text;
					break;
				case 313:
					cout << zr_text << '\n';
					message = zr_text;
					break;
				case 315:
					cout << plus_text << '\n';
					message = plus_text;
					break;
				case 314:
					cout << minus_text << '\n';
					message = minus_text;
					break;
				case 316:
					cout << home_text << '\n';
					message = home_text;
					break;
				case 309:
					cout << screenshot_text << '\n';
					message = screenshot_text;
					break;
				default:
					cout << ev.code << '\n';
			}	}
				
		} else if (ev.type == EV_ABS) {
			if (ev.code == ABS_HAT0X) {
				if (ev.value == -1) {
					cout << left_text << '\n';
					message = left_text;
				} else if (ev.value == 1) {
					cout << right_text << '\n';
					message = right_text;
				}/* else {
					cout << "ReleaseX\n";
				}*/
			}	
			if (ev.code == ABS_HAT0Y) {
				if (ev.value == -1) {
					cout << up_text << '\n';
					message = up_text;
				} else if (ev.value == 1) {
					cout << down_text << '\n';
					message = down_text;
				}/* else {
					cout << "ReleaseY\n";
				}*/
			}
		}
		if (message != ""){
			if (swap) {
				if (cycle) {
					
					message[0] = std::toupper(message[0]);
				}
				cycle = !cycle;
			}
			socket.write_some(boost::asio::buffer("PRIVMSG #" + channel + " :" + message + "\n"));
		}
	}}
	//std::string message = "Erfolg!";
	socket.write_some(boost::asio::buffer("PRIVMSG #" + channel + " :" + message + "\n"));
	
	std::string quit_message = "QUIT\r\n";
	socket.write_some(boost::asio::buffer(quit_message));
	socket.close();
	

	return 0;
}
