#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>

using namespace std;

string exec(const string& cmd)
{
    FILE* pipe = popen(cmd.c_str(), "r");

    if (!pipe)
        return "Unknown";

    char buffer[256];
    string result;

    while (fgets(buffer, sizeof(buffer), pipe))
        result += buffer;

    pclose(pipe);

    if (!result.empty() && result.back() == '\n')
        result.pop_back();

    return result;
}

string readFile(const string& path)
{
    ifstream file(path);

    if (!file)
        return "Logo not found";

    string line;
    string content;

    while (getline(file, line))
        content += line + "\n";

    return content;
}

string getOSField(const string& key)
{
    ifstream file("/etc/os-release");
    string line;

    while (getline(file, line))
    {
        if (line.rfind(key + "=", 0) == 0)
        {
            string value = line.substr(key.size() + 1);

            if (!value.empty() && value.front() == '"')
                value.erase(0, 1);

            if (!value.empty() && value.back() == '"')
                value.pop_back();

            return value;
        }
    }

    return "Unknown";
}

string distroToLogo(const string& distro)
{
    string path =
        "/usr/local/share/waterfetch/" +
        distro +
        ".txt";
    ifstream test(path);

    if (test)
        return path;

    return "logos/alpine.txt";
}

int getDistroColor(const string& distro)
{
    if(distro == "ubuntu")
        return 208;

    if(distro == "fedora")
        return 39;

    if(distro == "arch")
        return 51;

    if(distro == "linuxmint")
        return 48;

    if(distro == "debian")
        return 161;

    if(distro == "nixos")
        return 75;

    if(distro == "alpine")
        return 117;

    if(distro == "void")
        return 46;

    if(distro == "rhel")
        return 196;

    return 15;
}

int main(int argc, char* argv[])
{
    string distroID =
        getOSField("ID");

    bool fakeDistro = false;

    if(argc > 1)
    {
        string arg = argv[1];

        if(arg == "--help")
        {
            cout << "Waterfetch\n\n";

            cout << "Usage:\n";
            cout << "  waterfetch\n";
            cout << "  waterfetch --arch\n";
            cout << "  waterfetch --ubuntu\n";
            cout << "  waterfetch --fedora\n";
            cout << "  waterfetch --debian\n";
            cout << "  waterfetch --mint\n";
            cout << "  waterfetch --nixos\n";
            cout << "  waterfetch --void\n";
            cout << "  waterfetch --alpine\n";
            cout << "  waterfetch --rhel\n";

            return 0;
        }

        if(arg.rfind("--", 0) == 0)
        {
            distroID = arg.substr(2);
            fakeDistro = true;
        }
    }

    if(distroID == "mint")
        distroID = "linuxmint";

    string osName;

    if(fakeDistro)
    {
        if(distroID == "arch")
            osName = "Arch btw";
        else if(distroID == "ubuntu")
            osName = "Ubuntu";
        else if(distroID == "fedora")
            osName = "Fedora Linux";
        else if(distroID == "debian")
            osName = "Debian GNU/Linux";
        else if(distroID == "linuxmint")
            osName = "Linux Mint";
        else if(distroID == "alpine")
            osName = "Alpine Linux";
        else if(distroID == "nixos")
            osName = "NixOS";
        else if(distroID == "void")
            osName = "Void Linux";
        else if(distroID == "rhel")
            osName = "Red Hat Enterprise Linux";
        else
            osName = distroID;
    }
    else
    {
        osName =
            getOSField("PRETTY_NAME");
    }

    string kernel;

    if(fakeDistro)
    {
        if(distroID == "arch")
            kernel = "linux";

        else if(distroID == "ubuntu")
            kernel = "generic";

        else if(distroID == "fedora")
            kernel = "fc";

        else if(distroID == "debian")
            kernel = "amd64";

        else if(distroID == "linuxmint")
            kernel = "mint";

        else if(distroID == "nixos")
            kernel = "nixos";

        else if(distroID == "void")
            kernel = "void";

        else if(distroID == "alpine")
            kernel = "alpine";

        else if(distroID == "rhel")
            kernel = "rhel";

        else
            kernel = "linux";
    }
    else
    {
        kernel =
            exec("uname -r");
    }

    char hostname[256];
    gethostname(
        hostname,
        sizeof(hostname)
    );

    string user =
        getenv("USER")
        ? getenv("USER")
        : "unknown";

    string shell =
        getenv("SHELL")
        ? getenv("SHELL")
        : "unknown";

    string logo =
        readFile(
            distroToLogo(distroID)
        );

    vector<int> rainbow =
    {
        196,
        202,
        226,
        46,
        51,
        21,
        93,
        201
    };

    int finalColor =
        getDistroColor(
            distroID
        );

    cout << "\033[?25l";

    for(int frame = 0;
        frame < 20;
        frame++)
    {
        (void)system("clear");

        int color =
            rainbow[
                frame %
                rainbow.size()
            ];

        cout
            << "\033[38;5;"
            << color
            << "m";

        cout << logo;

        cout << "\033[0m";

        cout.flush();

        this_thread::sleep_for(
            chrono::milliseconds(
                100
            )
        );
    }

    (void)system("clear");

    cout
        << "\033[38;5;"
        << finalColor
        << "m";

    cout << logo;

    cout << "\033[0m\n";

    cout << user
         << "@"
         << hostname
         << "\n";

    cout << "--------------------------\n";

    cout << "OS      : "
         << osName
         << "\n";

    cout << "Kernel  : "
         << kernel
         << "\n";

    cout << "CPU     : "
         << exec(
             "grep 'model name' "
             "/proc/cpuinfo | "
             "head -1 | "
             "cut -d ':' -f2"
         )
         << "\n";

    cout << "RAM     : "
         << exec(
             "free -h | "
             "awk '/Mem:/ "
             "{print $3 "
             "\" / \" "
             "$2}'"
         )
         << "\n";

    cout << "Uptime  : "
         << exec("uptime -p")
         << "\n";

    cout << "Shell   : "
         << shell
         << "\n";

    cout << "\033[?25h";

    return 0;
}