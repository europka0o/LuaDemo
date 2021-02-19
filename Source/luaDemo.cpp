#include <iostream>
#include <LuaBridge/LuaBridge.h>
#include <vector>

using namespace luabridge;

class DATA {
    private:
        int value;
    public:
        DATA();
        void setValue(int a);
        int getValue();
        void printValue();
};

namespace kernel {
    void addToList(); //Добавляет новый элемент в вектор
    void delToList(); //Удаляет последний элемент в векторе
    static unsigned int getGlId(); //Возвращает глобальное число элементов
    DATA& getElem(int num); //Возвращает ссылку на элемент num в векторе

    static unsigned int global_id = 0;

    std::vector<DATA*> dt_list;
};

int main()
{
    system("chcp 1251");

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    //Добавляем в пространство lua класс DATA и все его методы, чтобы они знали друг о друге
    getGlobalNamespace(L). 
        beginClass<DATA>("DATA").
            addConstructor<void(*)()>().
            addFunction("setValue", &DATA::setValue).
            addFunction("printValue", &DATA::printValue).
            addFunction("getValue", &DATA::getValue).
        endClass();

    //Добавляем пространство имен kernel в lua
    getGlobalNamespace(L).
        beginNamespace("kernel").
            addFunction("addToList", kernel::addToList).
            addFunction("getGlId", kernel::getGlId).
            addFunction("getElem", kernel::getElem).
        endNamespace();

    luaL_dofile(L, "DemoL.lua"); //Запускаем скрипт lua
    lua_pcall(L, 0, 0, 0);

    LuaRef lua_buff_variable = getGlobal(L, "main"); 
    lua_buff_variable(); //Выполняем функцию main из lua скрипта

    std::cin.get();
    
    for (std::vector<DATA*>::iterator it = kernel::dt_list.begin(); it != kernel::dt_list.end();) { //Очищаем память
        delete *it;
        it = kernel::dt_list.erase(it);
    }

    return 0;
}

DATA::DATA() : value(0) {}

void DATA::setValue(int a) {
    value = a;
}

int DATA::getValue() {
    return value;
}

void DATA::printValue() {
    std::cout << value << std::endl;
}

void kernel::addToList() {
    dt_list.push_back(new DATA());
    global_id++;
}

void kernel::delToList() {
    if (global_id != 0) {
        dt_list.pop_back();
        global_id--;
    }
    
}

unsigned int kernel::getGlId() {
    return global_id;
}

DATA& kernel::getElem(int num) {
    return *(dt_list[num]);
}
