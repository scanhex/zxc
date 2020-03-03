#include <cstdio>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using namespace boost::posix_time;
io_service service;

class talk_to_client;
typedef boost::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> array;

#define MEM_FN(x)         boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)      boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)    boost::bind(&self_type::x, shared_from_this(),y,z)
#define MEM_FN3(x,y,z,w)  boost::bind(&self_type::x, shared_from_this(),y,z,w)

int connections_handled = 0;
int hp1 = 1000;
int hp2 = 1000;
int regen = 2;


class talk_to_client : public boost::enable_shared_from_this<talk_to_client>
                     , boost::noncopyable {
    typedef talk_to_client self_type;
    talk_to_client() : sock_(service), started_(false), 
                       timer_(service), clients_changed_(false) {
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start(int idx) {
        started_ = true;
        do_read(idx);
        do_write(idx);
    }
    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }
    bool started() const { return started_; }

    void stop() {
        if(!started_) return;
        sock_.close();
        --connections_handled;
        if(!connections_handled)
            started_=false;
    }
    ip::tcp::socket & sock() { return sock_;}
private:
    void on_read(const error_code & err, size_t bytes, int idx) {
        if(!started_) return;
        int dmg=read_buffer_[0];
        if(idx==0 && hp1>0){
            hp2-=dmg;
            if(hp2<0)
                hp2=0;
        }else{
            if(hp2>0){
            hp1-=dmg;
            if(hp1<0)
                hp1=0;
            }
            
        }
        if(hp1!=0 && hp2!=0)
            do_read(idx);
    }
   
    void do_read(int idx) {
        if(!started_) return;
        async_read(sock_, buffer(read_buffer_), 
                   MEM_FN2(read_complete,_1,_2), MEM_FN3(on_read,_1,_2,idx));
        
    }
    size_t read_complete(const boost::system::error_code & err, size_t bytes) {
        if ( err) return 0;
        bool done = (bytes>=1);
        return done ? 0 : 1;
    }
    void on_write(const error_code & err, size_t bytes, int idx) {
        if(hp1==0 || hp2==0) stop();
        if(!started_) return;
        int millis = 1000;
        timer_.expires_from_now(boost::posix_time::millisec(millis));
        timer_.async_wait( MEM_FN1(do_write,idx));
    }

    void do_write(int idx) {
        std::cout<<hp1<< ' '<<hp2<<std::endl;
        if(idx == 0){
            write_buffer_[0]=hp1/128;
            write_buffer_[1]=hp1%128;
            write_buffer_[2]=hp2/128;
            write_buffer_[3]=hp2%128;

        }else{
            write_buffer_[0]=hp2/128;
            write_buffer_[1]=hp2%128;
            write_buffer_[2]=hp1/128;
            write_buffer_[3]=hp1%128;
        }
        sock_.async_write_some(buffer(write_buffer_, 4), 
                                MEM_FN3(on_write,_1,_2,idx));
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string username_;
    deadline_timer timer_;
    bool clients_changed_;
};

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

void handle_accept(talk_to_client::ptr client, const boost::system::error_code & err) {
    client->start(connections_handled);
    connections_handled++;
    talk_to_client::ptr new_client = talk_to_client::new_();
    if(connections_handled<2)
        acceptor.async_accept(new_client->sock(), boost::bind(handle_accept,new_client,_1));
}

void game_state_run(){
    deadline_timer timer_(service);
    while(true){
        if(hp1>0)
            hp1+=regen;
        if(hp2>0)
            hp2+=regen;
        if(hp1>1000)
            hp1=1000;
        if(hp2>1000)
            hp2=1000;
        //std::cout<<"hp1="<<hp1<<" hp2="<<hp2<<std::endl;
        int millis = 100;
        timer_.expires_from_now(boost::posix_time::millisec(millis));
        timer_.wait();
    }
}

int main(int argc, char* argv[]) {
    talk_to_client::ptr client = talk_to_client::new_();
    acceptor.async_accept(client->sock(), boost::bind(handle_accept,client,_1));
    boost::thread_group threads;
    threads.create_thread(game_state_run);
    service.run();
    threads.join_all();
}