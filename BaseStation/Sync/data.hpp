#pragma once

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>

class DataStorage
{
    private:
        static DataStorage *singleton;
        
        sql::Driver *driver;
        sql::Connection *con;

        
    public:
        static DataStorage* getSingleton();

        int init();
        int getLastInsertID();
        int insertDevice(char* key,char* iv);
};