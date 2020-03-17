#include <mutex>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <set>

class html_parser{

private:
    int request_type; // get => 0 , post => 1 , put=> 2
    std::string url;
    std::string text;
    std::map<std::string,std::string> request_inputs;
    void url_parser(std::string url);
public:
    int get_request_type();
    std::string get_input(std::string a);
    std::string get_text();
    html_parser(char * buffer,int buffer_length);

};
