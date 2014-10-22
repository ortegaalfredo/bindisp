/*

  binstruct.cpp

  Este archivo guarda la estructura de un archivo y operaciones de export/import.
  No debe utilizar ningun modulo no-estandard para su facilidad de reuso.

  */

#include "binstruct.h"

#include <stdio.h>
#include <string>


void stdoutlog(const char *buf) {
printf("%s\n",buf);
}

void (*Log)(const char *) = stdoutlog;
//*****************************************************
// Crea un binario vacio
Binary::Binary(string path) {
        this->path=path;
    }

// Crea binario a partir de XML

Binary::Binary(string path,string xmlFile) {
        this->path=path;
        //this->Load(xmlFile);
}


int Binary::toXml(string xmlFile) {
xmlFile="TODO";
// TODO: IMPLEMENT TO/FROM XML
return 0;
}

// TODO: Real xrefs with strings,data, etc.
void Binary::updateXrefs(void) {
    int cops=0,cxrefs=0,crefsnotfound=0;
    char *buf;
    char conv[100]; //horrible
    Log("Updating Sub maps...");
    xrefmap.clear();
    for (unsigned int i=0;i<sections.size();i++)
     {
        Section *sec = sections[i];
        for (unsigned int q=0;q<sec->subs.size();q++)
            {
            Xref xr;
            Sub *sub = sec->subs[q];
            xr.object = sub;
            xr.type = BM_SUB;
            xr.label = "";
            xrefmap[sub->name.c_str()] = XrefSet();
            xrefmap[sub->name.c_str()].push_back(xr);
            sprintf(conv,"0x%llx",sub->StartBlock->vmastart);
            xrefmap[conv] = XrefSet();
            xrefmap[conv].push_back(xr);
            sub->children.clear();
            sub->parents.clear();

            }
    }
    asprintf( &buf, "Added %d Subs.",(int)xrefmap.size() );
    Log( buf );
    free(buf);

    Log("Updating xrefs...");
    for (unsigned int i=0;i<sections.size();i++)
     {
        Section *sec = sections[i];
        for (unsigned int q=0;q<sec->subs.size();q++)
            {
            Sub *sub = sec->subs[q];
            for (unsigned int r=0;r<sub->basicblocks.size();r++)
                {
                Block *block = sub->basicblocks[r];
                for (unsigned int x=0;x<block->opcodes.size();x++)
                    {
                    OpCode *opCode = block->opcodes[x];
                    // Extraemos nombre de funcion llamada
                    sprintf(conv,"0x%llx",opCode->target);
//                    XrefSet *xrs = &xrefmap[conv];

                    if (xrefmap.find(conv) != xrefmap.end()) {
                        Xref xr = xrefmap[conv].at(0);
                        if (xr.type != BM_SUB) break;
                        Sub *target = (Sub *)xr.object;
                        if (!target) break;
                        cxrefs++;
                        // Save the xref
                        sub->children.push_back(target);
                        target->parents.push_back(sub);
                        } else crefsnotfound++;

                    sprintf(conv,"0x%llx",opCode->target2);
                    //xrs = &xrefmap[conv];
                    if (xrefmap.find(conv) != xrefmap.end())  {
                        Xref xr = xrefmap[conv].at(0);
                        if (xr.type != BM_SUB) break;
                        Sub *target2 = (Sub *)xr.object;
                        if (!target2) break;
                        cxrefs++;
                        // Save the xref
                        sub->children.push_back(target2);
                        target2->parents.push_back(sub);
                        } else crefsnotfound++;
                    cops++;
                    }
                }
            }
     }
asprintf( &buf, "Traversed %d Ops, found %d xrefs, %d not found.",cops,cxrefs,crefsnotfound );
Log( buf );
free(buf);
}

//*****************************************************
Section::Section(string name)	{
this->name=name;
}

//*****************************************************
Sub::Sub(void)	{
this->name="";
this->data=NULL;
this->layout = LT_NONE;
}

//*****************************************************
Sub::Sub(string name)	{
this->name=name;
this->data=NULL;
this->layout = LT_NONE;
}

//*****************************************************
Block::Block(string name)	{
this->name=name;
tag=B1=B2= NULL;
type=BBT_NORMAL;
}

//*****************************************************
OpCode::OpCode(PBlock parent)	{
this->block = parent;
}

//*****************************************************
/* GetBasicBlock: Get basic block that contains the memory offset given in the section given,or NULL if no sub, or invalid offset */
PBlock GetBasicBlock(mem64 BasicBlockVma,PSection section,unsigned int maxDistance)
{
    PBlock retblock,choosenBlock;
    mem64 BasicBlockOffset = BasicBlockVma-section->vma;
    choosenBlock=NULL;
    int minDistance=99999999;
    /* sanity check */
    if (BasicBlockOffset>section->size)
       return NULL;
    /* Check is not in any sub */
    if ((section->bytesmap[BasicBlockOffset] !=  BM_SUB) && (section->bytesmap[BasicBlockOffset] !=  BM_LIB))
         return(NULL);
    /* Iterate all subs */
    for ( vector<PSub>::iterator psub = section->subs.begin();psub<section->subs.end();++psub )
        for (vector<PBlock>::iterator pblock = (*psub)->basicblocks.begin();pblock<(*psub)->basicblocks.end();++pblock )
            {
            retblock=*pblock;
            if (!maxDistance) // Return exact match, BasicBlockVma MUST be inside the block
                {
                if ((retblock->vmastart<=BasicBlockVma) && (BasicBlockVma<retblock->vmaend))
                    return retblock;
                }
            else if (std::abs( (long long int)(retblock->vmastart-BasicBlockVma))<maxDistance )
                    if (minDistance>retblock->vmastart-BasicBlockVma)
                        {
                        minDistance = retblock->vmastart-BasicBlockVma;
                        choosenBlock = retblock;
                        }
            }
    if (!maxDistance)
        return(NULL);
    else return choosenBlock;
}

// Given a VMA returns the section
PSection GetSection(mem64 BasicBlockVma,PBinary bin)
{
    for ( vector<PSection>::iterator psect = bin->sections.begin();psect<bin->sections.end();++psect )
        {
        if ( (((*psect)->vma) <= BasicBlockVma) &&  (((*psect)->vma+(*psect)->size) > BasicBlockVma) )
             return (*psect);
        }
    return NULL;
}

// Give a VMA returns the closest Basic Block
PBlock GetBasicBlock(mem64 BasicBlockVma,PBinary bin,unsigned int maxDistance)
{
    for ( vector<PSection>::iterator psect = bin->sections.begin();psect<bin->sections.end();++psect )
        {
        PBlock block = GetBasicBlock(BasicBlockVma,(*psect),maxDistance);
        if (block) return block;
        }
    return NULL;
}
