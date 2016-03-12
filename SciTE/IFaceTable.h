// SciTE - Scintilla based Text Editor
/** @file IFaceTable.h
 ** SciTE iface function and constant descriptors.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef IFACETABLE_H
#define IFACETABLE_H

enum IFaceType {
	iface_void,
	iface_int,
	iface_length,
	iface_position,
	iface_colour,
	iface_bool,
	iface_keymod,
	iface_string,
	iface_stringresult,
	iface_cells,
	iface_textrange,
	iface_findtext,
	iface_formatrange
};	

struct IFaceConstant {
	const char *name;
	int value;
};

struct IFaceFunction {
	const char *name;
	int value;
	IFaceType returnType;
	IFaceType paramType[2];
};

struct IFaceProperty {
	const char *name;
	int getter;
	int setter;
	IFaceType valueType;
	IFaceType paramType;
	
	IFaceFunction GetterFunction() const {
		IFaceFunction result = {"(property getter)",getter,valueType,{paramType,iface_void}};
		return result;
	}
	
	IFaceFunction SetterFunction() const {
		IFaceFunction result = {"(property setter)",setter,iface_void,{valueType, iface_void}};
		if (paramType != iface_void) {
			result.paramType[0] = paramType;
			if (valueType == iface_stringresult)
				result.paramType[1] = iface_string;
			else
				result.paramType[1] = valueType;
		}
		if ((paramType == iface_void) && ((valueType == iface_string) || (valueType == iface_stringresult))) {
			result.paramType[0] = paramType;
			if (valueType == iface_stringresult)
				result.paramType[1] = iface_string;
			else
				result.paramType[1] = valueType;
		}
		return result;
	}
};

class IFaceTableInterface {
public:
	virtual int FindConstant(const char *name) = 0;
	virtual int FindFunction(const char *name) = 0;
	virtual int FindFunctionByConstantName(const char *name) = 0;
	virtual int FindProperty(const char *name) = 0;
	virtual int GetConstantName(int value, char *nameOut, unsigned nameBufferLen, const char *hint) = 0;
};

class IFaceTable : public IFaceTableInterface {
public:

public:
	IFaceTable(const char *_prefix,
		const IFaceFunction *const _functions, int _functionCount,
		const IFaceConstant *const _constants, int _constantCount,
		const IFaceProperty *const _properties, int _propertyCount) :
		prefix(_prefix),
		functions(_functions), functionCount(_functionCount),
		constants(_constants), constantCount(_constantCount),
		properties(_properties), propertyCount(_propertyCount)
	{}

	const char *prefix;

	const IFaceFunction *const functions;
	const IFaceConstant *const constants;
	const IFaceProperty *const properties;

	const int functionCount;
	const int constantCount;
	const int propertyCount;

	int FindConstant(const char *name);
	int FindFunction(const char *name);
	int FindFunctionByConstantName(const char *name);
	int FindProperty(const char *name);

	int GetConstantName(int value, char *nameOut, unsigned nameBufferLen, const char *hint);
};

#endif
