#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <wx/wx.h>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include <boost/serialization/version.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <opencv2/opencv.hpp>
#include "../MyLog.h"

using namespace std;

//BOOST_SERIALIZATION_SPLIT_FREE(cv::Mat)
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void serialize(Archive &ar, cv::Mat& mat, const unsigned int)
		{
			int cols, rows, type;
			bool continuous;

			if (Archive::is_saving::value) {
				cols = mat.cols; rows = mat.rows; type = mat.type();
				continuous = mat.isContinuous();
			}

			ar & cols & rows & type & continuous;

			if (Archive::is_loading::value)
				mat.create(rows, cols, type);

			if (continuous) {
				const unsigned int data_size = rows * cols * mat.elemSize();
				ar & boost::serialization::make_array(mat.ptr(), data_size);
			}
			else {
				const unsigned int row_size = cols*mat.elemSize();
				for (int i = 0; i < rows; i++) {
					ar & boost::serialization::make_array(mat.ptr(i), row_size);
				}
			}
		}
	}
}

BOOST_SERIALIZATION_SPLIT_FREE(cv::Point)
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void save(Archive & ar, const cv::Point& p, const unsigned int version)
		{
			ar& p.x;
			ar& p.y;
		}

		template<class Archive>
		void load(Archive & ar, cv::Point& p, const unsigned int version)
		{
			ar& p.x;
			ar& p.y;
		}
	}
}


class Element
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int file_version)
	{
		ar& m_image_name;
		ar& m_image_mark;
		ar& m_image_point;
		ar& m_model_image;
	}
public:
	Element();
	virtual ~Element();

	//inline
	inline void SetImageName(string image_name)
	{
		m_image_name = image_name;
	};
	inline string GetImageName()
	{
		return m_image_name;
	};
	inline void SetImageMark(vector<cv::Mat> image_mark)
	{
		m_image_mark.assign(image_mark.begin(), image_mark.end());
	};
	inline list<cv::Mat> GetImageMark()
	{
		return m_image_mark;
	};
	inline void SetImagePoint(vector<cv::Point> image_point)
	{
		m_image_point.assign(image_point.begin(), image_point.end());
	};
	inline list<cv::Point> GetImagePoint()
	{
		return m_image_point;
	};
	inline void SetModelIamge(cv::Mat model_image)
	{
		m_model_image = model_image;
	};
	inline cv::Mat GetModelImage()
	{
		return m_model_image;
	};
private:
	string m_image_name;
	list<cv::Mat> m_image_mark;
	list<cv::Point> m_image_point;
	cv::Mat m_model_image;
};

class DataOrganization
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int file_version)
	{
		ar& m_name;
		ar& m_path;
		ar& m_element_list;
	}
public:
	DataOrganization(string v_name = "", string v_path = "");
	virtual ~DataOrganization();

	static DataOrganization *OnLoad(string v_file, string v_path);
	bool OnSave(string v_path = "");

	inline void SetElementList(list<Element*> v_list)
	{
		m_element_list.assign(v_list.begin(), v_list.end());
	};
	inline list<Element*> GetElementList()
	{
		return m_element_list;
	};
private:
	string m_name;
	string m_path;
	list<Element*> m_element_list;
};
//////////////////////////////////////////////////////////
BOOST_SERIALIZATION_SPLIT_FREE(wxRect)
namespace boost
{
	namespace serialization
	{
		template<class Archive>
		void save(Archive& ar, const wxRect& m, const unsigned int version)
		{
			ar& m.x;
			ar& m.y;
			ar& m.width;
			ar& m.height;
		}

		template<class Archive>
		void load(Archive& ar, wxRect& m, const unsigned int version)
		{
			ar& m.x;
			ar& m.y;
			ar& m.width;
			ar& m.height;
		}
	}
}

class ImageInfo
{
public:
	string imageName;
	string imageResult;
	string imageNgNum;
	string imagePath;
	string imageTime;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& imageName;
		ar& imageResult;
		ar& imageNgNum;
		ar& imagePath;
		ar& imageTime;
	}
};

class ListData
{
public:
	ListData(string path = "");
	virtual ~ListData();

	bool OnSave(string path = "");
	static ListData* OnLoad(string path);

	void SetListInfo(std::vector<ImageInfo> vec);
	std::vector<ImageInfo>& GetListInfo();
	void ClearData();

	void SetRedRectMap(std::map<string, list<wxRect>> rectMap);
	std::map<string, list<wxRect>>& GetRedRectMap();
	void ClearMapData();

private:
	std::vector<ImageInfo> m_imageInfoVec;
	std::map<string, list<wxRect>> m_redRectMap;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& m_imageInfoVec;
		ar& m_redRectMap;
	}
};