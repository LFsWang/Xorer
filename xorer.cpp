#include<iostream>
#include<algorithm>
#include<fstream>
constexpr size_t BLOCKSIZE = 4*1024*1024;

void showhelp()
{
    std::cout<<"xorer file Keys"<<std::endl;
    std::cout<<"xorer file -f KeyFiles"<<std::endl;
}

void human_filesize(std::size_t bytes)
{
    static char CH[] = "BKMGTP";
    int factor = (std::to_string(bytes).size()-1)/3 ;
    printf("%.2f%cb", double(bytes)/(1LL<<(10*factor)),CH[factor]);
}

char buffer[BLOCKSIZE];
bool xorFile(std::fstream &fio,std::string key)
{
    if( key.size() == 0 )
    {
        std::cout<<"Bad Key! (key size = 0)"<<std::endl;
        return false;
    }
    
    auto beginpos = fio.tellg();
    fio.seekg( 0,std::ios::end );
    
    std::size_t filesize = fio.tellg() - beginpos;
    fio.seekg( 0,std::ios::beg );
    
    for(std::size_t i=0,j=0 ; i<filesize ; i+=BLOCKSIZE)
    {
        std::size_t blocksz = std::min(BLOCKSIZE,filesize-i);
        printf("(%2.2f%%)",(double)(i+blocksz)/filesize*100 );
        human_filesize(i+blocksz); std::cout<<"/"; human_filesize(filesize); std::cout<<std::endl;

        auto before = fio.tellg();
        fio.read(buffer,blocksz);
        
        for(std::size_t k=0; k<blocksz; ++k )
        {
            buffer[k]^=key[j++];
            j%=key.size();
        }
        
        fio.seekg(before);
        fio.write(buffer,blocksz);
    }
    return true;
}

int main(int argc,char *argv[])
{
    std::fstream fio;
    std::string key;
    if( argc == 1 || argc >4 )
    {
        showhelp();
        return 0;
    }
    
    
    if( argc == 2 )
    {
        std::cout<<"Enter the XOR key:";
        getline(std::cin,key);
    }
    else if( argc == 3 )
    {
        key = argv[2];
    }
    else
    {
        std::cout<<"QQ"<<std::endl;
        showhelp();
        return 0;
    }

    fio.open( argv[1],std::ios::in | std::ios::out | std::ios::binary );
    if( !fio.is_open() )
    {
        std::cout<<"Open source file fail!";
        return -1;
    }
    xorFile( fio,key );
}
