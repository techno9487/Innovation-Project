#include "data.hpp"

DataStorage* DataStorage::singleton;

DataStorage* DataStorage::getSingleton()
{
    if(DataStorage::singleton == NULL)
    {
        DataStorage::singleton = new DataStorage;
    }
    return singleton;
}

int DataStorage::init()
{
    //get sql driver instance
    this->driver = get_driver_instance();
    if(driver == NULL)
    {
        printf("Failed to create driver instance\n");
        return -1;
    }

    //now try to connect
    this->con = this->driver->connect("127.0.0.1","root","password");
    if(con == nullptr)
    {
        printf("Failed to connect to the database\n");
        return -1;
    }

    //set the database we're working with
    con->setSchema("project");
    return 1;
}

int DataStorage::getLastInsertID()
{
    sql::PreparedStatement *statement;
    statement = con->prepareStatement("select LAST_INSERT_ID()");
    
    sql::ResultSet *res;
    res = statement->executeQuery();
    if(res == NULL)
    {
        return -1;
    }

    if(!res->next())
    {
        return -1;
    }

    return res->getInt(1);
}

int DataStorage::insertDevice(char *key,char *iv)
{
    sql::SQLString keyString(key);
    sql::SQLString ivString(iv);

    sql::PreparedStatement *statememt = con->prepareStatement("INSERT INTO Device VALUES (0,'',?,?)");
    statememt->setString(1,keyString);
    statememt->setString(2,ivString);
    return !statememt->execute();
}