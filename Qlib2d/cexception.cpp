#include "stdafx.h"
#include "cexception.h"


cexception::cexception(QString msg) noexcept : msg(msg)
{

}


cexception::~cexception()
{
}
