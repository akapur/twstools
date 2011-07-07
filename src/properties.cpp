
#include <cstdio>
#include <libconfig.h++>
#include "properties.h"
#include "debug.h"


const Properties* Properties::_properties = NULL;

const QString Properties::FILE_IO_ERROR_MSQ("file io error %1");
const QString Properties::PARSE_ERROR_MSG("parse error in %1: %2, line %3");
const QString Properties::SETTING_NOT_FOUND_MSG("setting not found in %1: %2");
const QString Properties::SETTING_TYPE_WRONG_MSG("wrong setting type (%1) in %2: %3");

Properties::Properties()
{
	if( _properties == NULL ) {
		conf = new libconfig::Config();
		_properties = this;
	} else {
		Q_ASSERT(false);
	}
}

Properties::~Properties()
{
	delete conf;
	Properties::_properties = NULL;
}

const Properties& Properties::globalProperties()
{
	Q_ASSERT( _properties != NULL );
	return *_properties;
}

bool Properties::readConfigFile( const QString &confFileName )
{
	QString errorMsg;
	try{
 		conf->readFile(confFileName.toAscii().data());
		this->confFileName = confFileName;
		conf->setAutoConvert(true);
		PROP_DEBUG( 1, QString("auto convert readConfigFile: %1")
			.arg(conf->getAutoConvert() ? "true" : "false") );
		return true;
	} catch(libconfig::FileIOException) {
		errorMsg = FILE_IO_ERROR_MSQ.arg(confFileName);
	} catch(libconfig::ParseException e ) {
		errorMsg = PARSE_ERROR_MSG.arg(confFileName).arg(e.getError()).arg(e.getLine());
	}
	PROP_DEBUG( 0, errorMsg );
	return false;
}

QByteArray Properties::getConfigBlob() const
{
	FILE* stream;
	int size;
	stream = tmpfile();
	conf->write(stream);
	size=ftell(stream);
	fseek(stream,0,SEEK_SET);
	QByteArray byteArray(size,0);
	fread(byteArray.data(),size,1,stream);
	fclose (stream);
	return byteArray;
}

bool Properties::exists(const QString &key) const
{
	return conf->exists(key.toAscii().data());
}

bool Properties::lookupValue(const QString &key, bool &value) const
{
	QString errorMsg;
	try {
		value = (bool) conf->lookup(key.toAscii().data());
		return true;
	} catch (libconfig::SettingNotFoundException) { 
		errorMsg = SETTING_NOT_FOUND_MSG;
	} catch(libconfig::SettingTypeException) {
		errorMsg = SETTING_TYPE_WRONG_MSG.arg("bool");
	}
	
	PROP_DEBUG( 3, errorMsg.arg(confFileName).arg(key) );
	return false;
}

bool Properties::lookupValue(const QString &key, int &value) const
{
	QString errorMsg;
	try {
		value = (int) conf->lookup(key.toAscii().data());
		return true;
	} catch (libconfig::SettingNotFoundException) { 
		errorMsg = SETTING_NOT_FOUND_MSG;
	} catch(libconfig::SettingTypeException) {
		errorMsg = SETTING_TYPE_WRONG_MSG.arg("int");
	}
	
	PROP_DEBUG( 3, errorMsg.arg(confFileName).arg(key) );
	return false;
}

bool Properties::lookupValue(const QString &key, QString &value) const
{
	QString errorMsg;
	try {
		value = (const char*) conf->lookup(key.toAscii().data());
		return true;
	} catch (libconfig::SettingNotFoundException) { 
		errorMsg = SETTING_NOT_FOUND_MSG;
	} catch(libconfig::SettingTypeException) {
		errorMsg = SETTING_TYPE_WRONG_MSG.arg("string");
	}
	
	PROP_DEBUG( 3, errorMsg.arg(confFileName).arg(key) );
	return false;
}

bool Properties::lookupValue(const QString &key, quint16 &value) const
{
	bool retVal;
	int  intTMP = value;
	
	retVal = lookupValue(key,intTMP);
	value = (quint16) intTMP;
	
	return retVal;
}


bool PropSub::exists(const QString& key) const
{
	return prop.exists(configName+"."+key);
}

bool PropSub::get(const QString& key, bool& value) const
{
	return prop.lookupValue(configName+"."+key,value);
}

bool PropSub::get(const QString& key, int& value) const
{
	return prop.lookupValue(configName+"."+key,value);
}

bool PropSub::get(const QString& key, QString& value) const
{
	return prop.lookupValue(configName+"."+key,value);
}

bool PropSub::get(const QString& key, quint16& value) const {
	return prop.lookupValue(configName+"."+key,value);
}




