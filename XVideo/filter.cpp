#include <QThread>
#include <QMutex>
#include <QObject>
#include "imagepro.h"
#include "filter.h"
using namespace cv;

Filter::Filter()
{

}

Filter::~Filter()
{

}

CXFilter::CXFilter()
{

}

CXFilter::~CXFilter()
{

}

Filter *Filter::Get()
{
    static CXFilter cx;
    return &cx;
}



