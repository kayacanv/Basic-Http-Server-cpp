all: main.cpp
	g++ -std=c++11 -pthread main.cpp html_parser.cpp -o server
