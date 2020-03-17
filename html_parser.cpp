#include "html_parser.h"


html_parser::html_parser(char * buffer,int buffer_length)
{
    if(buffer_length==0) return ;
    std::vector <std::string> lines;
    int header_ends=0;
    for (int i = 0; i < buffer_length; i++)
    {
        std::string a;
        while(i<buffer_length && buffer[i]!='\n' && buffer[i]!=EOF)
        {
            a+=buffer[i];
            i++;
        }
        if((a.size()==0 || a[0]=='\n') && header_ends==0)
        {
            header_ends=lines.size();
        }
        lines.push_back(a);
    }
    std::string tmp[3];
    int tmp_cnt=0;
    for (int i = 0; i < lines[0].size(); i++)
    {
        if(lines[0][i]==' ')
            tmp_cnt++;
        else
            tmp[tmp_cnt].push_back(lines[0][i]);
    }
                
    if(tmp[0]=="GET")
        request_type = 0;
    else if(tmp[0]=="POST")
        request_type = 1;
    else if(tmp[0]=="PUT")
        request_type = 2;

    url = tmp[1];
    if(request_type==0)
        url_parser(url);
    if(request_type!=0)
    {
        int t=buffer_length--;
        while(t>=0 && buffer[t]!='\n' && buffer[t]!='=')
            t--;
        t++;  
        while(t<buffer_length)
        {
            text+=buffer[t];
            t++;
        }
        text+=buffer[t];
    }
}


int html_parser:: get_request_type()
{
    return request_type;
}
void html_parser::url_parser(std::string url)
{
    if(url.find("name")==std::string::npos) return ;
    request_inputs["name"] = url.substr(url.find("name")+5);
}
std::string html_parser:: get_input(std::string a)
{
    if(request_inputs.count(a)==0) return "";
    return request_inputs[a];
}
std::string html_parser::get_text()
{
    return text;
}
