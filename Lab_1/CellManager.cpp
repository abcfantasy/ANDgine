#include "CellManager.h"
#include "rapidxml.hpp"
#include <string>
#include <fstream>
#include <vector>

using namespace rapidxml;

// singleton instance
CellManager* CellManager::instance()
{
	static CellManager cm;
	return &cm;
}

int CellManager::LoadCell( char* fileName, int cellX, int cellY )
{
	// get section
	char cellText[9];
	sprintf( cellText, "cell%dx%d", cellX, cellY );

	std::string input_xml;
	std::string line;
	std::ifstream xmlFile(fileName);

	// read file into string
	while ( std::getline(xmlFile, line) )
		input_xml += line;

	// make a safe-to-modify copy of input_xml
	// (never modify contents of std::string directly
	std::vector<char> text_xml( input_xml.begin(), input_xml.end() );
	text_xml.push_back( '\0' );

	// only use the text_xml from here onwards
	xml_document<> doc;
	// parse xml declaration
	// pare_no_data_nodes prevents RapidXML from using the somwhat
	// suprising behaviour of having boh values and data nodes, and having
	// data nodes take precedence over values when printing
	// Note: this will skip parsing CDATA nodes
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&text_xml[0]);

}