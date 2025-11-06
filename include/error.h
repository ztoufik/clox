#ifndef ERROR_H
#define ERROR_H

#include<string>

namespace tua{

    struct Error{
            Error(std::string&& msg,std::uint32_t lnum):_msg(std::move(msg)),_lnum(lnum){}
            std::string _msg;
            std::uint32_t _lnum;

            virtual operator std::string() const=0;
    };

    struct ParseError:Error{
            ParseError(std::string&& msg,std::uint32_t lnum):Error(std::move(msg),lnum){}

            virtual operator std::string() const override{
                return _msg+" : "+std::to_string(_lnum);
            }

            bool operator==(const ParseError& err ) const {
                return !((bool)(_msg.compare(err._msg))) && _lnum==err._lnum;
            }
    };
};


#endif 
