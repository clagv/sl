#include "SLUtils.h"
#include "SLData.h"
#include "SLVector.h"
#include "SLMap.h"
#include "SLSet.h"
#include "SLPair.h"
#include <iostream>
#include <iterator>
#include <map>

using namespace sl;

typedef std::vector<std::string> StrVec;

struct Lvl1
{
    std::string m_a;
    std::string m_b;
    StrVec      m_vec;
};

ISerializablePtr makeSerializable(const std::string& key, Lvl1& lvl1)
{
    ISerializableObjPtr wrt = createSerializableObj(key);
    add(wrt, "a", lvl1.m_a);
    add(wrt, "b", lvl1.m_b);
    add(wrt, "vec", lvl1.m_vec);
    return wrt;
}

struct Lvl2
{
    std::string m_c;
    Lvl1        m_lvl1;
    std::vector<StrVec> m_vecVec;
    std::map<std::string, StrVec> m_map;
};

ISerializablePtr makeSerializable(const std::string& key, Lvl2& lvl2)
{
    ISerializableObjPtr wrt = createSerializableObj(key);
    add(wrt, "c", lvl2.m_c);
    add(wrt, makeSerializable("1", lvl2.m_lvl1));
    add(wrt, "vv", lvl2.m_vecVec);
    add(wrt, makeSerializable("map", lvl2.m_map));
    return wrt;
}

int main()
{
    std::pair<const std::string, std::string> css("abc", "d");
    std::pair<std::string, std::string> ss("ef","gh");
    std::pair<StrVec, std::string> svs(StrVec(2, "blah"), "ij");
    ISerializablePtr serPair;
    serPair = makeSerializable("", ss);
    serPair = makeSerializable("", svs);
    Lvl2 lvl2;
    ISerializablePtr ser = makeSerializable("", lvl2);
    lvl2.m_lvl1.m_b = "abc";
    lvl2.m_lvl1.m_vec.assign(2, "vec0");
    lvl2.m_vecVec.assign(3, StrVec(3, "vv0"));
    lvl2.m_map["mapKey"] = StrVec(2, "mapVal");
    IDataPtr dt = createEmptyData();
    ser->write(dt.get());
    dt->getDataWrite("1", 0)->setVal("a", "blah", 0);
    dt->getDataWrite("1", 0)->getDataWrite("vec", 0)->setVal("item", "vec1", 1);
    dt->getDataWrite("vv", 0)->getDataWrite("item", 2)->setVal("item", "vv22", 2);
    dt->getDataWrite("map", 0)->getDataWrite("mapKey2")->setVal("item", "mapVal2");
    dt->setVal("c", "OK", 0);
    ser->read(dt.get());
    std::cout << lvl2.m_lvl1.m_a << '\n';
    std::cout << lvl2.m_c << '\n';
    std::copy(lvl2.m_lvl1.m_vec.begin(), lvl2.m_lvl1.m_vec.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << '\n' << lvl2.m_vecVec.at(2).at(2) << '\n';
    std::cout << (++lvl2.m_map.begin())->first << '\n';
    std::set<std::string> strSet;
    makeSerializable("", strSet);
    IData* pd = dt->getDataWrite("vv", 0)->getDataWrite("item", 2);
    pd->begin();
    pd->next();
    pd->erase("item", 2);
    if(pd->end())
        std::cout << "erase error";
    pd->erase("item", 1);
    if(!pd->end())
        std::cout << "erase error";
    return 0;
}