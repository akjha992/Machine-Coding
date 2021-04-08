#include <bits/stdc++.h>
using namespace std;
class Row{
    private:
    vector<string> data;
    public:
    Row(){}
    Row(vector<string> data):data(data){}
    vector<string> getRow(){
        return data;
    }
    void print(){
        for(string item:data){
            cout<<item<<" ";
        }
        cout<<endl;
    }
};

class ColumnSchema{
    private:
    string type; //upgrade to enum if required
    bool required;
    bool validateString(string data){
        //  string type can have a maximum length of 20 characters
        return data.size()<=20;
    }
    bool validateInt(string data){
        stringstream ss(data); 
        //throws error on unsuccessful conversion
        int val;
        ss>>val;
        return val>=-1024&&val<=1024;
    }
    public: 
    ColumnSchema(){}
    ColumnSchema(string type, bool required): type(type), required(required){}
    bool validate(string data){
        if(required&&data==""){
            return false;
        }
        if(type=="int"){
            return validateInt(data);
        }else if(type=="string"){
            return validateString(data);
        }
    }

};

class Schema{
    private:
    vector<string> columnNames;
    vector<ColumnSchema> columnSchemas;
    public:
    Schema(){}
    Schema(vector<string> columnNames, vector<ColumnSchema> columnSchemas):columnNames(columnNames), columnSchemas(columnSchemas){}
    ColumnSchema getColumnSchema(int index){
        return columnSchemas[index];
    }
    int getIndexFromColumnName(string columnName){
        int index=-1;
        for(int i=0;i<columnNames.size();i++){
            if(columnNames[i]==columnName){
                index=i;
                break;
            }
        }
        return index;
    }
};
class Table{
    private:
    Schema schema;
    map<int,Row> rows;
    public:
    Table(){}
    Table(Schema schema):schema(schema){}
    static int count;
    void insert(vector<string> data){
        bool validationSuccessful=true;
        for(int i=0;i<data.size();i++){
            if(!schema.getColumnSchema(i).validate(data[i])){
                validationSuccessful=false;
                break;
            }
        }
        if(!validationSuccessful){
            cout<<"Invalid row data!"<<endl;
            return;
        }
        Row newRow(data);
        rows[++count]=data;
    }
    vector<Row> getAll(){
        vector<Row> result;
        for(auto it:rows){
            result.push_back(it.second);
        }
        return result;
    }
    vector<Row> getWithFilter(string colName , string filter){
        int index = schema.getIndexFromColumnName(colName);
        if(index==-1){
            return getAll();
        }
        vector<Row> result;
        for(auto it:rows){
            bool filterIn = false;
            if(it.second.getRow()[index]==filter){
                result.push_back(it.second);
            }
        }
        return result;
    }
};
int Table::count = 0;

class Database{
    private:
    map<string, Table> tables;
    public:
    Table createNewTable(string tableName, Schema schema){
        if(tables.count(tableName)){
            cout<<"Table already exists!";
        }else{
            Table newTable(schema);
            tables[tableName]=newTable;
        }
        return tables[tableName];
    }
};

class DatabaseManager{
    private:
    map<string, Database> databases;
    public:
    Database createNewDatabase(string databaseName){
        if(databases.count(databaseName)){
            cout<<"Database already exists!";
        }else{
            Database newDataabse;
            databases[databaseName]=newDataabse;
        }
        return databases[databaseName];
    }
};
int main(){
    DatabaseManager dm;
    Database db = dm.createNewDatabase("mydb");

    
    Schema mySchema({"Name", "Age"}, {ColumnSchema("string", true), ColumnSchema("int", false)});

    Table myTable = db.createNewTable("myTable", mySchema);
    
    cout<<"Inserting Valid elements"<<endl;
    myTable.insert({"Ashish", "24"});
    myTable.insert({"Abhishek", "24"});
    myTable.insert({"Raman", "34"});
    cout<<endl<<endl;

    cout<<"Inserting Invalid elements"<<endl;
    myTable.insert({"Rahul", "134134"});//check for int limits
    myTable.insert({"", "12"});//check for required
    myTable.insert({"Ramdfwhbjehwbdfdfwhjefwhjk", "12"});//check for string limits
    cout<<endl<<endl;

    cout<<"Printing all table elements"<<endl;
    for(Row row: myTable.getAll()){
        row.print();
    }
    cout<<endl<<endl;

    cout<<"Printing all table elements where age=24"<<endl;
    for(Row row: myTable.getWithFilter("Age", "34")){
        row.print();
    }
    cout<<endl<<endl;
}