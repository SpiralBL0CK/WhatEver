#include <string>
#include <iostream>
#include <fstream>      
using namespace std;

/*Example to serialize struct from a dist file */
struct Object{
    char *name;
    public:
        void set_values(char * x);
};

void Object::set_values ( char * x)
{
  name = x;
}


void write(const std::string& file_name,Object& data)
{
    std::ofstream out;
	out.open(file_name,std::ios::binary);
	out.write(reinterpret_cast<char*>(&data), sizeof(Object));
	out.close();
}

void read(const std::string& file_name, Object& data) // Reads the given file and assigns the data to the given OBJECT.
{
	std::ifstream in;
	in.open(file_name,std::ios::binary);
	in.read(reinterpret_cast<char*>(&data), sizeof(Object));
	in.close();
};

int main()
{
    Object rect;
    read("a.txt",rect);
    cout << rect.name;
    return 0;
}