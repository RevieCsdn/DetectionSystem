#include "DataElement.h"

Element::Element()
	:m_image_name(""), m_model_image({})
{
	m_image_mark.clear();
	m_image_point.clear();
}

Element::~Element()
{

}

//================================================

DataOrganization::DataOrganization(string v_name /* = "" */, string v_path /* = "" */)
{
	m_name = v_name;
	m_path = v_path;
	m_element_list.clear();
}

DataOrganization::~DataOrganization()
{

}

DataOrganization *DataOrganization::OnLoad(string v_file, string v_path)
{
	DataOrganization *d = new DataOrganization("", "");
	std::ifstream ifs(v_path, std::ios::binary);
	boost::archive::binary_iarchive ia(ifs);
	ia >> *d;
	ifs.close();

	wxString msg = wxString::Format("文件已导入配方，路径为：%s", v_path);
	MyLog::LogMessage(msg.c_str());

	return d;
}

bool DataOrganization::OnSave(string v_path /* = "" */)
{
	std::ofstream ofs(v_path, std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << *this;
	ofs.close();

	wxString msg = wxString::Format("配方已导出文件，路径为：%s", v_path);
	MyLog::LogMessage(msg.c_str());

	return true;
}
/////////////////////////////////

ListData::ListData(string path)
{
}

ListData::~ListData()
{

}

bool ListData::OnSave(string path)
{
	std::ofstream ofs(path, std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << *this;
	ofs.close();

	return true;
}

ListData * ListData::OnLoad(string path)
{
	ListData *l_listData = new ListData;
	std::ifstream ifs(path, std::ios::binary);
	boost::archive::binary_iarchive ia(ifs);
	ia >> *l_listData;
	ifs.close();

	return l_listData;

}

void ListData::SetListInfo(std::vector<ImageInfo> vec)
{
	m_imageInfoVec = vec;
}

std::vector<ImageInfo>& ListData::GetListInfo()
{
	return m_imageInfoVec;
}

void ListData::ClearData()
{
	m_imageInfoVec.clear();
}

void ListData::SetRedRectMap(std::map<string, list<wxRect>> rectMap)
{
	m_redRectMap = rectMap;
}

std::map<string, list<wxRect>>& ListData::GetRedRectMap()
{
	return m_redRectMap;
}

void ListData::ClearMapData()
{
	m_redRectMap.clear();
}

/////////////////////////////////////////////////

NoTestElement::NoTestElement()
{
}

NoTestElement::~NoTestElement()
{
}


//
NoTestData::NoTestData(string name, string path)
	:m_name(name),m_path(path)
{
}

NoTestData::~NoTestData()
{
}

bool NoTestData::OnSave(string path)
{
	std::ofstream ofs(path, std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << *this;
	ofs.close();

	return true;
}

NoTestData* NoTestData::OnLoad(string name,string path)
{
	NoTestData *l_noTestData = new NoTestData;
	std::ifstream ifs(path, std::ios::binary);
	boost::archive::binary_iarchive ia(ifs);
	ia >> *l_noTestData;
	ifs.close();

	return l_noTestData;
}
///////////////
HasBeenTest::HasBeenTest(string name, string path)
{
}

HasBeenTest::~HasBeenTest()
{
}

bool HasBeenTest::OnSave(string path)
{
	std::ofstream ofs(path, std::ios::binary);
	boost::archive::binary_oarchive oa(ofs);
	oa << *this;
	ofs.close();

	return true;
}

HasBeenTest * HasBeenTest::OnLoad(string path)
{
	HasBeenTest *l_testData = new HasBeenTest;
	std::ifstream ifs(path, std::ios::binary);
	boost::archive::binary_iarchive ia(ifs);
	ia >> *l_testData;
	ifs.close();

	return l_testData;
}
