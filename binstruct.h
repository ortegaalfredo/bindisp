#ifndef BINSTRUCT_H
#define BINSTRUCT_H

#include <vector>
#include <string.h>

#ifdef __GNUC__
#define _BACKWARD_BACKWARD_WARNING_H //Fuck you C++!!
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include <map>


namespace std
{
 using namespace __gnu_cxx;
}

using namespace std;

// Enums
enum OpCodeType { OT_UNKNOWN = 0, OT_BRANCH=1, OT_LOGIC=1, OT_LOADSTORE=2 };
enum byteMap {BM_UNDEFINED = 0, BM_CODE = 1, BM_SUB = 2, BM_DATA = 3, BM_LIB = 4, BM_BYTE = 5, BM_SHORT = 6, BM_DWORD = 7, BM_QWORD = 8, BM_STRING = 9};
enum LayoutType { LT_NONE = 0, LT_GRAPHVIZ = 1, LT_OGDF=2};
enum BasicBlockType { BBT_NORMAL = 0, BBT_PROLOG = 1, BBT_EPILOG=2, BBT_BRANCH=4};

// Forward declarations
class Binary;
typedef Binary *PBinary;

class Section;
typedef Section *PSection;

class Symbol;
typedef Symbol *PSymbol;

class Sub;
typedef Sub *PSub;

class Block;
typedef Block *PBlock;

class OpCode;
typedef OpCode *POpCode;


struct eqstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

// 64 bit type
typedef unsigned long long int mem64;

// Xref definitions

typedef struct {
    string label;
    byteMap type;
    void *object;
} Xref;
typedef Xref *PXref;
typedef std::vector<Xref>      XrefSet;
typedef std::map<std::string, XrefSet> XrefSetMap;


class Binary {
        public:
                // Constructor vacio
                Binary(string path);
                // Construye desde un archivo XML
                Binary(string path,string xmlFile);
                // Carga un archivo XML
                int Load(string xmlFile);
                // Guarda en un archivo XML
                int toXml(string xmlFile);
                // Calcula referencias cruzadas
                void updateXrefs(void);
                // Mapa nombre->PSub
                //map <std::string, PSub> subMap;
                XrefSetMap xrefmap;
                // Entry point
                mem64 entryPoint;
                // Secciones
                vector <PSection> sections;
                string name;
                string path;
                string type;
                // Funciones de entry o export
                vector <PSub> exports;
                // Funciones importadas
                vector <PSub> imports;
                // Para visualizacion
                vector <void *> views;
        private:
        };

class Section {
        public:
                /* constructor */
                Section(string name);
                string name;
                PBinary binary;
                /* vma of first byte */
                mem64 vma;
                unsigned int size;
                /* Subs vector */
                vector <PSub> subs;
                /* map of opcodes type (lenght == this.size)*/
                unsigned char *bytes;
                unsigned char *bytesmap;
        private:
        };

class Sub {
        public:
                Sub();
                Sub(string name);
                PSymbol symbol;
                PSection section;
                string name;
                long long int offset;
                vector <PBlock> basicblocks;
                PBlock StartBlock;
                vector <PSub> children;
                vector <PSub> parents;
                LayoutType layout;
                // Additional generic data pointer
                void *data;
        private:
        };

class Block {
        public:
                Block(string name);
                string name;
                // Address
                mem64 vmastart,vmaend;
                // Opcodes
                vector <POpCode> opcodes;
                // See BasicBlockType
                int type;
                // parent sub
                PSub sub;
                // for branchs
                PBlock B1,B2;
                // for GUI
                void *tag;
        };

class OpCode {
        public:
                OpCode(PBlock parent);
                // Body of the opcode
                string content;
                // Address
                mem64 vma;
                // Max 2 targets
                mem64 target;
                mem64 target2;
                // Type
                OpCodeType type;
                // Code buffer
                int len;
                unsigned char *bytes;
                // parent block
                PBlock block;
        };

// Given a VMA returns the section
PSection GetSection(mem64 BasicBlockVma,PBinary bin);
/* GetBasicBlock: Get basic block that contains the memory offset given in the section given,or NULL if no sub, or invalid offset */
PBlock GetBasicBlock(mem64 BasicBlockVma, PSection section, unsigned int maxDistance = 0);
PBlock GetBasicBlock(mem64 BasicBlockVma,PBinary bin,unsigned int maxDistance = 0);
#endif // BINSTRUCT_H
