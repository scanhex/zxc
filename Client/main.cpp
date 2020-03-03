#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
io_service service;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_svr : public boost::enable_shared_from_this<talk_to_svr>
                  , boost::noncopyable {
    typedef talk_to_svr self_type;
    talk_to_svr(const std::string & username) 
      : sock_(service), started_(true), username_(username), timer_(service) {}
    void start(ip::tcp::endpoint ep) {
        sock_.async_connect(ep, MEM_FN1(on_connect,_1));
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_svr> ptr;

    static ptr start(ip::tcp::endpoint ep, const std::string & username) {
        ptr new_(new talk_to_svr(username));
        new_->start(ep);
        return new_;
    }
    void stop() {
        if ( !started_) return;
        std::cout << "stopping " << username_ << std::endl;
        started_ = false;
        sock_.close();
    }
    bool started() { return started_; }
private:
    void on_connect(const error_code & err) {
        if ( !err) {
            do_write();
            do_read();
        }else  
            stop();
    }


    void on_write(const error_code & err, size_t bytes) {
        int millis = rand() % 3000;
        timer_.expires_from_now(boost::posix_time::millisec(millis));
        timer_.async_wait( MEM_FN(do_write));
    }

    void do_write() {
        if ( !started() ) return;
        char val = rand() % 128;
       // std::cout<<username_<<" damaged "<<(int)val<<std::endl;
        write_buffer_[0]=val;
        sock_.async_write_some( buffer(write_buffer_, 1), 
                                MEM_FN2(on_write,_1,_2));
    }
    void on_read(const error_code & err, size_t bytes) {
        int hp1=read_buffer_[0]*128+read_buffer_[1];
        int hp2=read_buffer_[2]*128+read_buffer_[3];
        if(username_=="Maxim")
        std::cout<< username_<<": my hp is "<<hp1<<", enemy hp is "<<hp2<<std::endl;
        if(hp1==0 || hp2==0){
            if(hp1==0)
                std::cout<<username_<<": I lost :("<<std::endl;
            else
            std::cout<<username_<<": I won :)"<<std::endl;
            stop();
        }
        do_read();
    }

    void do_read() {
        if ( !started() ) return;
        async_read(sock_, buffer(read_buffer_), 
            MEM_FN2(read_complete,_1,_2), MEM_FN2(on_read,_1,_2));
    }

    size_t read_complete(const boost::system::error_code & err, size_t bytes) {
        if ( err) return 0;
        bool done = (bytes>=4);
        return done ? 0 : 1;
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string username_;
    deadline_timer timer_;
};

int main(int argc, char* argv[]) {
    // connect several clients
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
    char* names[] = { "Maxim", "Sasha", 0 };
    srand(228);
    for ( char ** name = names; *name; ++name) {
        talk_to_svr::start(ep, *name);
        boost::this_thread::sleep( boost::posix_time::millisec(100));
        std::cout<<"Connect "<<name<<std::endl;
    }
    service.run();
}