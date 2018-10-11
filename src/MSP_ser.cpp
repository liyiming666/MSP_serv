#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include <iomanip>

using namespace std;
using namespace boost::asio;

int count = 0;
static struct itimerval oldtv;

io_service ioserv;
static ip::tcp::socket socket_(ioserv);
void set_timer()
{
    struct itimerval itv;//结构体的定义在Timer.h
    itv.it_interval.tv_sec = 5;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 5;
    itv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, &oldtv);//向内核注册一个timer信号
	printf("__________________0___________________\n");
}

void signal_handler(int m)
{
	
	FILE *fd = NULL;
	fd = fopen("./o.out_bak","r");
	if(fd == NULL){
			perror("open file error.\n");
			fclose(fd);
			fd = NULL;
			return;
	}
	string str ;
	unsigned char *buf = NULL;
	size_t line = 0;
	map <int ,vector<string> > m_Vector;
	vector<string> test_Vector;
	int times = 0;
	while(1)
	{
			cout<<"line:"<<line <<endl;
			size_t t = getline((char **)&buf,&line,fd);
			cerr << "t: " << t << endl;
			if(t == -1 || feof(fd)){ //函数返回非0,则文件结束。
					perror("file end.\n");
					fclose(fd);
					fd = NULL;
					break;
			}
			test_Vector = { (char *)buf, to_string(t)};
			m_Vector.insert(pair<int , vector<string>>(times,test_Vector));
			times++;
	}
	//cerr << "times: " << times << endl;
	//exit(0);
	map<int , vector<string>>::iterator it = m_Vector.begin();
	cerr << m_Vector.size() <<endl;
	std::ostringstream out;
	for(int i =0 ;it != m_Vector.end(); it++){
/*
			out << std::hex << std::setw(2) << std::setfill('0'); 	//
			for (int t = 0; t < atoi(it->second[1].c_str()); t++) {
					out << static_cast<int>(it->second[0][t]) << " "; //
			}
			//cerr << it->first << "--" << out.str() <<"-----" << it->second[1] <<endl;
*/					
			boost::system::error_code ec;
			int t =	atoi((it->second[1]).c_str());
			printf("____t: %d____\n",t);

			//char p[]={0x0a,0x01,0x00,0x00,0x61,0x7b,0x22,0x63,0x6d,0x64,0x22,0x3a,0x22,0x75,0x70,0x64,0x61,0x74,0x61,0x22,0x2c,0x22,0x64,0x65,0x76,0x65,0x75,0x69,0x22,0x3a,0x22,0x34,0x61,0x37,0x37,0x30,0x32,0x62,0x63,0x30,0x30,0x31,0x63,0x30,0x38,0x22,0x2c,0x22,0x70,0x61,0x79,0x6c,0x6f,0x61,0x64,0x22,0x3a,0x22,0x61,0x48,0x4d,0x68,0x67,0x46,0x41,0x67,0x51,0x69,0x41,0x4e,0x47,0x41,0x63,0x44,0x45,0x41,0x4d,0x42,0x49,0x41,0x6c,0x6a,0x41,0x41,0x41,0x6f,0x4e,0x48,0x6b,0x57,0x22,0x2c,0x22,0x70,0x6f,0x72,0x74,0x22,0x3a,0x31,0x7d,0x00
//};

			//socket_.write_some(boost::asi/o::buffer(it->second[0], t), ec);
			char p[1000];
			cout<< it->second[0]<<endl;
			int count = 0;
			p[count++] = '\n';
			p[count++] = 0x00;
			p[count++] = 0x00;
			p[count++] = (t>>8)&0xFF;
			p[count++] = t&0xFF;
			for (int i = 0; i < t; i++) {
				p[count++] = it->second[0][i];
			}
			p[count++] = '\0';
			
/*
			int p_len = sprintf(p,"\n%02x%02x%02x%s\0",0x00,0x00,t,it->second[0]);
			cout<< p_len <<endl;

			p_len = t + 5;
*/
			for(int i = 0 ;i<count;i++){
				printf("%02x ",(unsigned char*)p[i]);
			}
			printf("\n");
			//socket_.write_some(boost::asio::buffer(it->second[0], t), ec);
			socket_.write_some(boost::asio::buffer(p, count), ec);
			if(!ec){
					printf("___________________2___________________\n");
					sleep(1);
			} else {
					cout<<boost::system::system_error(ec).what()<<endl;
					break;
			}
	}
}

int main()
{
	ip::tcp::acceptor acceptor(ioserv , ip::tcp::endpoint(ip::tcp::v4(),10000));

	acceptor.accept(socket_);
	printf("__________________1___________________\n");
	//socket.read_some();
	
	/*  内核收到setitemer时触发的信号
		会激活SIGALRM signal这个函数是信号量注册
		只要收到SIGALRM就会调用signal_handler
	*/
    signal(SIGALRM, signal_handler);
    set_timer();
    while(1);
	
	return 0;
}
