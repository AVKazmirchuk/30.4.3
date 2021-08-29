#include <iostream>
#include <string>
#include <map>
#include <cpr/cpr.h>
#include <vector>

std::string input(std::map<std::string, std::string>& args)
{
    std::string strArg, strVal;

    for (int i{}; ; ++i) {
        std::cout << "Argument " << i << ": ";
        std::cin >> strArg;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (strArg[0] >= 'a' && strArg[0] <= 'z' ||
                strArg[0] >= 'A' && strArg[0] <= 'Z') {
            if (strArg == "get" || strArg == "post") return strArg;

            std::cout << "Value " << i << ": ";
            std::cin >> strVal;
            args.insert({strArg, strVal});
        }
        else
        {
            std::cout << "Invalid argument name!\n";
            --i;
        }
    }
}

void doGet(std::map<std::string, std::string>& args, cpr::Response& r, std::string& strQuery)
{
    strQuery += "get?";

    for (auto it {begin(args)}; it != end(args); ++it)
    {
        strQuery += (*it).first + '=' + (*it).second + '&';
    }

    strQuery.pop_back();

    r = cpr::Get(cpr::Url(strQuery));
}

void doPost(std::map<std::string, std::string>& args, cpr::Response& r, std::string& strQuery)
{
    strQuery += "post";

    std::vector<cpr::Pair> tmp;

    for (auto it{begin(args)}; it != end(args); ++it) {
        tmp.emplace_back((*it).first.c_str(), (*it).second.c_str());
    }

    r = cpr::Post(cpr::Url(strQuery), cpr::Payload(tmp.begin(), tmp.end()));
}

int main()
{
    std::map<std::string, std::string> args;

    cpr::Response r;

    std::string strQuery {"http://httpbin.org/"};

    std::string method = input(args);

    if (method == "get") doGet(args, r, strQuery);
    else doPost(args, r, strQuery);

    std::cout << r.text;
}