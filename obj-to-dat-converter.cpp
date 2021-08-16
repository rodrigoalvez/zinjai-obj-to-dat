#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <conio.h>

using namespace std;

struct Node {
	float x,y,z;
	
	Node(float x, float y,float z){
		this->x = x;
		this->y = y;
		this->z = z;
	};	
};

struct Element {
	int nv;
	int n[4];
	Element(int n0 = 0, int n1 = 0, int n2 = 0, int n3 = -1) {
		n[0] = n0; n[1] = n1; n[2] = n2; n[3] = n3; nv = n3<0 ? 3 : 4;
	};
};

// Determinates the node's index according to an face's edge.
int getNode(string s){
	size_t p = s.find('/');
	stringstream s_int(s.substr(0,p));
	int i;
	s_int >> i;
	return i-1;
};

/// Function that determinates the node chains.
void nodeChain(string &ss,vector<string> &s){
	// We first clean the string vector.
	s.clear();
	
	// Then define the initial and final points.
	unsigned p0 = 0, pf = 0;
	
	while(ss.find(' ',p0) != string::npos){
		pf = ss.find(' ',p0);
		s.push_back(ss.substr(p0,pf-p0));
		p0 = pf+1;
	};
	
	s.push_back(ss.substr(p0));
};

/// Function that reads the .obj file.
bool readObjFile(string filename,vector<Node> &nodes,vector<Element> &elements){
	/// We first read the file.
	ifstream inFile("filesToConvert/" + filename + ".obj");
	if(!inFile.is_open()){
		cerr << "Error, the file '"<<filename<<".obj'"<<" doesn't couldn't be found."<<endl;
		return false;
	};
	
	//Auxiliar variables
	char quote[20];
	float x,y,z;
	string ss;
	vector<string> s;
	
	/// Reading loop
	while(inFile >> quote){
		/// Vertices' copying.
		if(string(quote) == "v"){
			// We read the coordinates.
			inFile >> x >> y >> z;
			// Then we add the nodes.
			nodes.push_back(nodes(x,y,z));
			// And finally we jump to the next line.
			inFile.ignore();
		};
		/// Faces' copying.
		else if(string(quote) == "f"){
			inFile.ignore();
			// We read the information of the faces' points.
			getline(inFile,ss);
			
			// Then we set the chain in a node array.
			nodeChain(ss,s);
			// And we add the faces.
			if(s.size()==3)	elements.push_back(Elemento(getNode(s[0]), getNode(s[1]), getNode(s[2])));
			else			elements.push_back(Elemento(getNode(s[0]), getNode(s[1]), getNode(s[2]), getNode(s[3])));
		}
		/// If the quote's invalid we jump to the next line.
		else {
			string s;
			getline(inFile,s);
		};
	};
	
	return true;
};

/// Function that creates the file to be read using C++.
void createDatFile(string filename, vector<Node> &nodes,vector<Element> &elements){
	// First we create the file.
	ofstream outFile("filesToConvert/" + filename + ".dat", ios::binary | ios::trunc);
	// Then write the nodes.
	outFile << nodes.size() << endl;
	for(size_t i=0;i<nodes.size();i++){
		// Coordinates (x, y, z).
		outFile << nodes[i].x << ' ' << nodes[i].y << ' ' << nodes[i].z << endl;
	}
	// Then write the faces.
	outFile << elements.size() << endl;
	for(size_t i=0;i<elements.size();i++){
		// We write the amount of vertices.
		outFile << elements[i].nv;
		// Then the edges from 0 to nv
		for(int j=0;j<elements[i].nv;j++) outFile << ' ' << elements[i].n[j];
		outFile << endl;
	}
	outFile.close();
};
	
int main() {
	
	cout<<"Welcome to the Blender's .obj to .dat File Converter."<<endl;
	cout<<"Press ESC to quit or any other key to transform a model file."<<endl<<endl;
	
	while(getch() != 27) {
		cout<<"Before starting please copy the desired files to the 'filesToConvert' folder.\n"
			<<"Then enter the file's name without the '.obj' extension to begin the transformation."<<endl;
		
		//Reading the .obj file.
		string filename;
		getline(cin, filename);
		
		//Vectors used in the procedure.
		vector<Node> nodes;
		vector<Element> elements;
		
		//Copying the file's data.
		cout<<endl<<"Reading "<<filename<<".obj..."<<endl;
		/// if(!LeeArchivoObj(Nombre,nodes,Elementos))	return -1;
		cout<<"Reading finished. Information loaded in memory."<<endl<<endl;
		
		//Creating the .dat file.
		cout<<"Creating "<<filename<<".dat file..."<<endl;
		/// CrearArchivoDat(Nombre,nodes,Elementos);
		cout<<"File created successfully."<<endl<<endl;
		
		cout<<"Press ESC to quit or any other key to transform a model file."<<endl<<endl;
	};
	
	return 0;
};
