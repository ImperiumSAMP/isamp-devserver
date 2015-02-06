/*
 * @author Teprey
 * @version	0.2
 *
 * CSTL plugin for SA:MP by Teprey (C) 2011
 * Edited Some bugs by Cyber_Punk (C) 2012
 * Provides some STL functions/data containers for SA:MP PAWN language 
 * Do not remove this author credit
 */

/* Entry types */
#define CSTL_TYPE_CELL 1
#define CSTL_TYPE_PTR 2
#define CSTL_TYPE_FLOAT 3

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

#include "AMX_SDK/amx/amx.h"
#include "AMX_SDK/plugincommon.h"

extern void * pAMXFunctions;
using namespace std;  

typedef struct {
	cell entry;
	string strentry;
	int type;
	
} entry;

bool operator ==(const entry& lhs, const entry& rhs)
{
	if (lhs.entry == NULL || rhs.entry == NULL)
	{
		return (lhs.strentry == rhs.strentry);
	}
	return (lhs.entry == rhs.entry);
}

static int cstl_index = 0;
static map<AMX*, map<cell, vector<entry> > > vectors;
static map<cell, AMX*> isVectorGlobalized;

static map<AMX*, map<cell, map<string, entry> > > maps;
static map<cell, AMX*> isMapGlobalized;

static map<AMX*, map<cell, deque<entry> > > deques;
static map<cell, AMX*> isDequeGlobalized;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) 
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    cout << "cstl by Teprey edited by Cyber_Punk(C) 2011 - 2012";
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload( )
{

}

// DEQUE

/* native deque_push_back(deqid, element); */
static cell AMX_NATIVE_CALL n_deque_push_back( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.entry = element;
	n.type = CSTL_TYPE_CELL;
	deques[amx][deqid].push_back(n);

	return 1;
}

/* native deque_push_back_float(deqid, Float:element); */
static cell AMX_NATIVE_CALL n_deque_push_back_float( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.entry = element;
	n.type = CSTL_TYPE_FLOAT;
	deques[amx][deqid].push_back(n);

	return 1;
}

/* native deque_push_back_arr(deqid, arr[]); */
static cell AMX_NATIVE_CALL n_deque_push_back_arr( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	char * element;
	amx_StrParam(amx,params[2],element);
	entry n;
	n.strentry = element;
	n.type = CSTL_TYPE_PTR;
	deques[amx][deqid].push_back(n);

	return 1;
}

/* native deque_push_front(deqid, element); */
static cell AMX_NATIVE_CALL n_deque_push_front( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.entry = element;
	n.type = CSTL_TYPE_CELL;
	deques[amx][deqid].push_front(n);

	return 1;
}

/* native deque_push_front_float(deqid, Float:element); */
static cell AMX_NATIVE_CALL n_deque_push_front_float( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.entry = element;
	n.type = CSTL_TYPE_FLOAT;
	deques[amx][deqid].push_front(n);

	return 1;
}

/* native deque_push_front_arr(deqid, arr[]); */
static cell AMX_NATIVE_CALL n_deque_push_front_arr( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	char * element;
	amx_StrParam(amx,params[2],element);
	entry n;
	n.strentry = element;
	n.type = CSTL_TYPE_PTR;
	deques[amx][deqid].push_front(n);

	return 1;
}

/* native deque_swap(deqid1, id1, dequeid2, id2); */
static cell AMX_NATIVE_CALL n_deque_swap( AMX* amx, cell* params )
{
	if (params[0] != 16) { return -1; /* insufficient parameters */ }
	cell deqid1 = params[1];
	cell deqid2 = params[3];
	if (isDequeGlobalized[deqid1] != NULL) { amx = isDequeGlobalized[deqid1]; } /* get AMX address if global */
	if (isDequeGlobalized[deqid2] != NULL) { amx = isDequeGlobalized[deqid2]; }
	cell element1 = params[2];
	cell element2 = params[4];
	entry tmp = deques[amx][deqid1][element1];
	deques[amx][deqid1][element1] = deques[amx][deqid2][element2];
	deques[amx][deqid2][element2] = tmp;

	return 1;
}

/* native deque_size(deqid); */
static cell AMX_NATIVE_CALL n_deque_size( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	return deques[amx][deqid].size();
}

/* native deque_get(deqid, id); */
static cell AMX_NATIVE_CALL n_deque_get( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell id = params[2];
	cell result = -1;
	if (deques[amx][deqid][id].type != CSTL_TYPE_CELL)
	{
		return -1;
	}
	try{
		result = deques[amx][deqid].at(id).entry;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}

	return result;
}

/* native deque_get_float(deqid, id); */
static cell AMX_NATIVE_CALL n_deque_get_float( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell id = params[2];
	cell result = -1;
	if (deques[amx][deqid][id].type != CSTL_TYPE_FLOAT)
	{
		return -1;
	}
	try{
		result = deques[amx][deqid].at(id).entry;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}

	return result;
}

/* native deque_get_arr(deqid, id, buffer[], buflen); */
static cell AMX_NATIVE_CALL n_deque_get_arr( AMX* amx, cell* params )
{
	if (params[0] != 16) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell id = params[2];
	if (deques[amx][deqid][id].type != CSTL_TYPE_PTR) 
	{
		return -1;
	}

	char* result = NULL;
	try{
		result = (char*)deques[amx][deqid].at(id).strentry.c_str();
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}

	cell *dest;
    amx_GetAddr(amx,params[3],&dest);
    amx_SetString(dest,(const char*)(result),0,0,params[4] + 1);
	return strlen((const char*)result) + 1;
}

/* native deque_set(deqid, id, element); */
static cell AMX_NATIVE_CALL n_deque_set( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell id = params[2];
	cell element = params[3];

	try{
		entry n;
		n.entry = element;
		n.type = CSTL_TYPE_CELL;
		deques[amx][deqid].at(id) = n;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}
	return 1;
}
/* native deque_set_float(deqid, id, element); */
static cell AMX_NATIVE_CALL n_deque_set_float( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell id = params[2];
	cell element = params[3];

	try{
		entry n;
		n.entry = element;
		n.type = CSTL_TYPE_FLOAT;
		deques[amx][deqid].at(id) = n;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}
	return 1;
}


/* native deque_set_arr(deqid, id, element[]); */
static cell AMX_NATIVE_CALL n_deque_set_arr( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }

	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell id = params[2];
	char * element;
	amx_StrParam(amx,params[3],element);

	
	entry n;
	n.strentry = element;
	n.type = CSTL_TYPE_PTR;
	try {
		deques[amx][deqid].at(id) = n;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}
	return 1;
}

/* native deque_clear(deqid); */
static cell AMX_NATIVE_CALL n_deque_clear( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	deques[amx][deqid].clear();
	return 1;
}


/* native deque_resize(deqid, newsize); */
static cell AMX_NATIVE_CALL n_deque_resize( AMX* amx, cell* params )
{
	/*+ attention: realloc with reducing size may cause memory leak ! */
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell newsize = params[2];
	deques[amx][deqid].resize(newsize);
	return 1;
}

/* native deque_find(deqid, element); */
static cell AMX_NATIVE_CALL n_deque_find( AMX* amx, cell* params )
{
	/*+ attention: realloc with reducing size may cause memory leak ! */
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.type = CSTL_TYPE_CELL;
	n.entry = element;
	deque<entry>::iterator it =  find(deques[amx][deqid].begin(), deques[amx][deqid].end(), n);
	if (it == deques[amx][deqid].end())
	{
		return -1;
	} else {
		return distance(deques[amx][deqid].begin(),it);
	}
}
/* native deque_find_float(deqid, Float:element); */
static cell AMX_NATIVE_CALL n_deque_find_float( AMX* amx, cell* params )
{
	/*+ attention: realloc with reducing size may cause memory leak ! */
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.type = CSTL_TYPE_FLOAT;
	n.entry = element;
	deque<entry>::iterator it =  find(deques[amx][deqid].begin(), deques[amx][deqid].end(), n);
	if (it == deques[amx][deqid].end())
	{
		return -1;
	} else {
		return distance(deques[amx][deqid].begin(),it);
	}
}
/* native deque_find_arr(deqid, element[]); */
static cell AMX_NATIVE_CALL n_deque_find_arr( AMX* amx, cell* params )
{
	/*+ attention: realloc with reducing size may cause memory leak ! */
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	char * element;
	amx_StrParam(amx,params[2],element);
	entry n;
	n.type = CSTL_TYPE_CELL;
	n.strentry = element;
	n.entry = NULL;
	deque<entry>::iterator it =  find(deques[amx][deqid].begin(), deques[amx][deqid].end(), n);
	if (it == deques[amx][deqid].end())
	{
		return -1;
	} else {
		return distance(deques[amx][deqid].begin(),it);
	}
}
/* native deque_remove(deqid, id); */
static cell AMX_NATIVE_CALL n_deque_remove( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell element = params[2];
	try {
	deques[amx][deqid].erase(deques[amx][deqid].begin() + element);
	} catch(exception) {}
	return 1;
}
/* native deque_entry_type(deqid, id); */
static cell AMX_NATIVE_CALL n_deque_entry_type( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell id = params[2];
	if (deques[amx][deqid][id].type == 0 || deques[amx][deqid][id].type == NULL)
	{
		return -1;
	}
	return deques[amx][deqid][id].type;
}
/* native deque_globalize(deqid); */
static cell AMX_NATIVE_CALL n_deque_globalize( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { 
		/* already global, return FALSE*/
		return -1; 
	} 

	isDequeGlobalized[deqid] = amx;
	return 1;
}
/* native deque_deglobalize(deqid); */
static cell AMX_NATIVE_CALL n_deque_deglobalize( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] == NULL) { 
		/* already local, return FALSE*/
		return -1; 
	} 
	/* copy vector into local and remove original */
	deque<entry> _new(deques[isDequeGlobalized[deqid]][deqid]);
	deques[amx][deqid] = _new;
	isDequeGlobalized[deqid] = NULL;
	return 1;
}
/* native deque_is_globalized(deqid); */
static cell AMX_NATIVE_CALL n_deque_is_globalized( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] == NULL) { 
	
		return -1; 
	} 
	return 1;
}
/* native deque_index_exists(deqid, index); */
static cell AMX_NATIVE_CALL n_deque_index_exists( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell deqid = params[1];
	if (isDequeGlobalized[deqid] != NULL) { amx = isDequeGlobalized[deqid]; } /* get AMX address if global */
	cell element = params[2];
	try {
	deques[amx][deqid].at(element).entry;
	} catch(out_of_range& oor) { cerr << "Out of Range error: " << oor.what() << endl; return -1; }
	return 1;
}

// VECTOR

/* native vector_push_back(vecid, element); */
static cell AMX_NATIVE_CALL n_vector_push_back( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.entry = element;
	n.type = CSTL_TYPE_CELL;
	vectors[amx][vecid].push_back(n);

	return 1;
}

/* native vector_push_back_float(vecid, Float:element); */
static cell AMX_NATIVE_CALL n_vector_push_back_float( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.entry = element;
	n.type = CSTL_TYPE_FLOAT;
	vectors[amx][vecid].push_back(n);

	return 1;
}


/* native vector_push_back_arr(vecid, arr[]); */
static cell AMX_NATIVE_CALL n_vector_push_back_arr( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	char * element;
	amx_StrParam(amx,params[2],element);
	entry n;
	n.strentry = element;
	n.type = CSTL_TYPE_PTR;
	vectors[amx][vecid].push_back(n);

	return 1;
}


/* native vector_size(vecid); */
static cell AMX_NATIVE_CALL n_vector_size( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	return vectors[amx][vecid].size();
}


/* native vector_get(vecid, id); */
static cell AMX_NATIVE_CALL n_vector_get( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell id = params[2];
	cell result = -1;
	if (vectors[amx][vecid][id].type != CSTL_TYPE_CELL)
	{
		return -1;
	}
	try{
		result = vectors[amx][vecid].at(id).entry;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}

	return result;
}
/* native Float:vector_get_float(vecid, id); */
static cell AMX_NATIVE_CALL n_vector_get_float( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell id = params[2];
	cell result = -1;
	if (vectors[amx][vecid][id].type != CSTL_TYPE_FLOAT)
	{
		return -1;
	}
	try{
		result = vectors[amx][vecid].at(id).entry;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}

	return result;
}

/* native vector_get_arr(vecid, id, buffer[], buflen); */
static cell AMX_NATIVE_CALL n_vector_get_arr( AMX* amx, cell* params )
{
	if (params[0] != 16) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell id = params[2];
	if (vectors[amx][vecid][id].type != CSTL_TYPE_PTR) 
	{
		return -1;
	}

	char* result = NULL;
	try{
		result = (char*)vectors[amx][vecid].at(id).strentry.c_str();
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}

	cell *dest;
    amx_GetAddr(amx,params[3],&dest);
    amx_SetString(dest,(const char*)(result),0,0,params[4] + 1);
	return strlen((const char*)result) + 1;
}


/* native vector_set(vecid, id, element); */
static cell AMX_NATIVE_CALL n_vector_set( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell id = params[2];
	cell element = params[3];

	try{
		entry n;
		n.entry = element;
		n.type = CSTL_TYPE_CELL;
		vectors[amx][vecid].at(id) = n;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}
	return 1;
}
/* native vector_set_float(vecid, id, element); */
static cell AMX_NATIVE_CALL n_vector_set_float( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell id = params[2];
	cell element = params[3];

	try{
		entry n;
		n.entry = element;
		n.type = CSTL_TYPE_FLOAT;
		vectors[amx][vecid].at(id) = n;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}
	return 1;
}


/* native vector_set_arr(vecid, id, element[]); */
static cell AMX_NATIVE_CALL n_vector_set_arr( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }

	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell id = params[2];
	char * element;
	amx_StrParam(amx,params[3],element);

	
	entry n;
	n.strentry = element;
	n.type = CSTL_TYPE_PTR;
	try {
		vectors[amx][vecid].at(id) = n;
	}
	catch(out_of_range& oor){
		cerr << "Out of Range error: " << oor.what() << endl; return -1;
	}
	return 1;
}

/* native vector_clear(vecid); */
static cell AMX_NATIVE_CALL n_vector_clear( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	vectors[amx][vecid].clear();
	return 1;
}


/* native vector_resize(vecid, newsize); */
static cell AMX_NATIVE_CALL n_vector_resize( AMX* amx, cell* params )
{
	/*+ attention: realloc with reducing size may cause memory leak ! */
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell newsize = params[2];
	vectors[amx][vecid].resize(newsize);
	return 1;
}

/* native vector_find(vecid, element); */
static cell AMX_NATIVE_CALL n_vector_find( AMX* amx, cell* params )
{
	/*+ attention: realloc with reducing size may cause memory leak ! */
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.type = CSTL_TYPE_CELL;
	n.entry = element;
	vector<entry>::iterator it =  find(vectors[amx][vecid].begin(), vectors[amx][vecid].end(), n);
	if (it == vectors[amx][vecid].end())
	{
		return -1;
	} else {
		return distance(vectors[amx][vecid].begin(),it);
	}
}
/* native vector_find_float(vecid, Float:element); */
static cell AMX_NATIVE_CALL n_vector_find_float( AMX* amx, cell* params )
{
	/*+ attention: realloc with reducing size may cause memory leak ! */
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell element = params[2];
	entry n;
	n.type = CSTL_TYPE_FLOAT;
	n.entry = element;
	vector<entry>::iterator it =  find(vectors[amx][vecid].begin(), vectors[amx][vecid].end(), n);
	if (it == vectors[amx][vecid].end())
	{
		return -1;
	} else {
		return distance(vectors[amx][vecid].begin(),it);
	}
}
/* native vector_find_arr(vecid, element[]); */
static cell AMX_NATIVE_CALL n_vector_find_arr( AMX* amx, cell* params )
{
	/*+ attention: realloc with reducing size may cause memory leak ! */
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	char * element;
	amx_StrParam(amx,params[2],element);
	entry n;
	n.type = CSTL_TYPE_CELL;
	n.strentry = element;
	n.entry = NULL;
	vector<entry>::iterator it =  find(vectors[amx][vecid].begin(), vectors[amx][vecid].end(), n);
	if (it == vectors[amx][vecid].end())
	{
		return -1;
	} else {
		return distance(vectors[amx][vecid].begin(),it);
	}
}
/* native vector_remove(vecid, id); */
static cell AMX_NATIVE_CALL n_vector_remove( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell element = params[2];
	try {
	vectors[amx][vecid].erase(vectors[amx][vecid].begin() + element);
	} catch(exception) {}
	return 1;
}
/* native vector_entry_type(vecid, id); */
static cell AMX_NATIVE_CALL n_vector_entry_type( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell id = params[2];
	if (vectors[amx][vecid][id].type == 0 || vectors[amx][vecid][id].type == NULL)
	{
		return -1;
	}
	return vectors[amx][vecid][id].type;
}
/* native vector_globalize(vecid); */
static cell AMX_NATIVE_CALL n_vector_globalize( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { 
		/* already global, return FALSE*/
		return -1; 
	} 

	isVectorGlobalized[vecid] = amx;
	return 1;
}
/* native vector_deglobalize(vecid); */
static cell AMX_NATIVE_CALL n_vector_deglobalize( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] == NULL) { 
		/* already local, return FALSE*/
		return -1; 
	} 
	/* copy vector into local and remove original */
	vector<entry> _new(vectors[isVectorGlobalized[vecid]][vecid]);
	vectors[amx][vecid] = _new;
	isVectorGlobalized[vecid] = NULL;
	return 1;
}
/* native vector_is_globalized(vecid); */
static cell AMX_NATIVE_CALL n_vector_is_globalized( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] == NULL) { 
	
		return -1; 
	} 
	return 1;
}
/* native vector_index_exists(vecid, index); */
static cell AMX_NATIVE_CALL n_vector_index_exists( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell vecid = params[1];
	if (isVectorGlobalized[vecid] != NULL) { amx = isVectorGlobalized[vecid]; } /* get AMX address if global */
	cell element = params[2];
	try {
	vectors[amx][vecid].at(element).entry;
	} catch(out_of_range& oor) { cerr << "Out of Range error: " << oor.what() << endl; return -1; }
	return 1;
}
// MAP
/* native map_insert(mapid, key[], value); */
static cell AMX_NATIVE_CALL n_map_insert( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	cell element = params[3];
	entry n;
	n.entry = element;
	n.type = CSTL_TYPE_CELL;

	if(key == NULL) return -1;
	maps[amx][mapid][key] = n;
	return 1;
}

/* native map_insert_float(mapid, key[], Float:value); */
static cell AMX_NATIVE_CALL n_map_insert_float( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	cell element = params[3];
	entry n;
	n.entry = element;
	n.type = CSTL_TYPE_FLOAT;

	if(key == NULL) return -1;
	maps[amx][mapid][key] = n;
	return 1;
}
/* native map_insert_arr(mapid, key[], value[]); */
static cell AMX_NATIVE_CALL n_map_insert_arr( AMX* amx, cell* params )
{
	if (params[0] != 12) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	char * element;
	amx_StrParam(amx,params[3],element);
	entry n;
	n.strentry = element;
	n.type = CSTL_TYPE_PTR;

	if(key == NULL) return -1; 
	maps[amx][mapid][key] = n;
	return 1;
}
/* native map_get(mapid, key[]); */
static cell AMX_NATIVE_CALL n_map_get( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	if(key == NULL) return -1;
	if (maps[amx][mapid].find(key) == maps[amx][mapid].end() || maps[amx][mapid][key].type != CSTL_TYPE_CELL) { return -1; }
	return maps[amx][mapid][key].entry;
}
/* native Float:map_get_float(mapid, key[]); */
static cell AMX_NATIVE_CALL n_map_get_float( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	if(key == NULL) return -1;
	if (maps[amx][mapid].find(key) == maps[amx][mapid].end() || maps[amx][mapid][key].type != CSTL_TYPE_FLOAT) { return -1; }
	return maps[amx][mapid][key].entry;
}
/* native map_get_arr(mapid, key[], buffer[], buflen); */
static cell AMX_NATIVE_CALL n_map_get_arr( AMX* amx, cell* params )
{
	if (params[0] != 16) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	if(key == NULL) return -1;
	if (maps[amx][mapid].find(key) == maps[amx][mapid].end() || maps[amx][mapid][key].type != CSTL_TYPE_PTR) { return -1; }
	const char* result = maps[amx][mapid][key].strentry.c_str();

	cell *dest;
    amx_GetAddr(amx,params[3],&dest);
    amx_SetString(dest,(const char*)(result),0,0,params[4] + 1);
	return strlen((const char*)result) + 1;
}
/* native map_size(mapid); */
static cell AMX_NATIVE_CALL n_map_size( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	return maps[amx][mapid].size();
}
/* native map_remove(mapid, key[]); */
static cell AMX_NATIVE_CALL n_map_remove( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	if(key == NULL) return -1;
	if(maps[amx][mapid].find(key) == maps[amx][mapid].end()) return -1;
	maps[amx][mapid].erase(key);
	return 1;
}
/* native map_contains(mapid, key[]); */
static cell AMX_NATIVE_CALL n_map_contains( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	if(key == NULL) return -1;
	if (maps[amx][mapid].find(key) == maps[amx][mapid].end()) { return 0; }
	return 1;
}
/* native map_entry_type(mapid, key[]); */
static cell AMX_NATIVE_CALL n_map_entry_type( AMX* amx, cell* params )
{
	if (params[0] != 8) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	char * key = NULL;
	amx_StrParam(amx,params[2],key);
	if(key == NULL) return -1;
	if (maps[amx][mapid][key].type == 0 || maps[amx][mapid][key].type == NULL)
	{
		return -1;
	}
	return maps[amx][mapid][key].type;
}
/* native map_clear(mapid); */
static cell AMX_NATIVE_CALL n_map_clear( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { amx = isMapGlobalized[mapid]; } /* get AMX address if global */
	maps[amx][mapid].clear();
	return 1;
}
/* native map_globalize(vecid); */
static cell AMX_NATIVE_CALL n_map_globalize( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] != NULL) { 
		/* already global, return FALSE*/
		return -1; 
	} 

	isMapGlobalized[mapid] = amx;
	return 1;
}
/* native map_deglobalize(vecid); */
static cell AMX_NATIVE_CALL n_map_deglobalize( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] == NULL) { 
		/* already local, return FALSE*/
		return -1; 
	} 
	/* copy vector into local and remove original */
	map<string, entry> _new(maps[isMapGlobalized[mapid]][mapid]);
	maps[amx][mapid] = _new;
	isMapGlobalized[mapid] = NULL;
	return 1;
}
/* native map_is_globalized(vecid); */
static cell AMX_NATIVE_CALL n_map_is_globalized( AMX* amx, cell* params )
{
	if (params[0] != 4) { return -1; /* insufficient parameters */ }
	cell mapid = params[1];
	if (isMapGlobalized[mapid] == NULL) { 
	
		return -1; 
	} 
	return 1;
}

AMX_NATIVE_INFO hashAMXNatives[ ] =
{
	/* deque */
	{"deque_push_back", n_deque_push_back},
	{"deque_push_back_float", n_deque_push_back_float},
	{"deque_push_back_arr", n_deque_push_back_arr},
	{"deque_push_front", n_deque_push_front},
	{"deque_push_front_float", n_deque_push_front_float},
	{"deque_push_front_arr", n_deque_push_front_arr},
	{"deque_swap", n_deque_swap},
	{"deque_size", n_deque_size},
	{"deque_get", n_deque_get},
	{"deque_get_float", n_deque_get_float},
	{"deque_get_arr", n_deque_get_arr},
	{"deque_set", n_deque_set},
	{"deque_set_float", n_deque_set_float},
	{"deque_set_arr", n_deque_set_arr},
	{"deque_clear", n_deque_clear},
	{"deque_resize", n_deque_resize},
	{"deque_entry_type", n_deque_entry_type},
	{"deque_find",n_deque_find},
	{"deque_find_float",n_deque_find_float},
	{"deque_find_arr",n_deque_find_arr},
	{"deque_remove",n_deque_remove},
	{"deque_globalize", n_deque_globalize},
	{"deque_deglobalize", n_deque_deglobalize},
	{"deque_is_globalized", n_deque_is_globalized},
	{"deque_index_exists", n_deque_index_exists},
	/* vector */
	{"vector_push_back", n_vector_push_back},
	{"vector_push_back_float", n_vector_push_back_float},
	{"vector_push_back_arr", n_vector_push_back_arr},
	{"vector_size", n_vector_size},
	{"vector_get", n_vector_get},
	{"vector_get_float", n_vector_get_float},
	{"vector_get_arr", n_vector_get_arr},
	{"vector_set", n_vector_set},
	{"vector_set_float", n_vector_set_float},
	{"vector_set_arr", n_vector_set_arr},
	{"vector_clear", n_vector_clear},
	{"vector_resize", n_vector_resize},
	{"vector_entry_type", n_vector_entry_type},
	{"vector_find",n_vector_find},
	{"vector_find_float",n_vector_find_float},
	{"vector_find_arr",n_vector_find_arr},
	{"vector_remove",n_vector_remove},
	{"vector_globalize", n_vector_globalize},
	{"vector_deglobalize", n_vector_deglobalize},
	{"vector_is_globalized", n_vector_is_globalized},
	{"vector_index_exists", n_vector_index_exists},
	/* map */
	{"map_insert", n_map_insert},
	{"map_insert_float", n_map_insert_float},
	{"map_insert_arr", n_map_insert_arr},
	{"map_get", n_map_get},
	{"map_get_float", n_map_get_float},
	{"map_get_arr", n_map_get_arr},
	{"map_size", n_map_size},
	{"map_remove",n_map_remove},
	{"map_contains",n_map_contains},
	{"map_entry_type",n_map_entry_type},
	{"map_clear",n_map_clear},
	{"map_globalize", n_map_globalize},
	{"map_deglobalize", n_map_deglobalize},
	{"map_is_globalized", n_map_is_globalized},
    {0,                0}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
    return amx_Register( amx, hashAMXNatives, -1 );
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	for (map<cell, vector<entry> >::iterator it = vectors[amx].begin() ; it != vectors[amx].end() ; it++)
	{
		(*it).second.clear();
	}
	vectors[amx].clear();
	for (map<cell, map<string, entry> >::iterator it = maps[amx].begin() ; it != maps[amx].end() ; it++)
	{
		(*it).second.clear();
	}
	maps[amx].clear();

    return AMX_ERR_NONE;
}
