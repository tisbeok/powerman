/*****************************************************************************\
 *  $Id$
 *****************************************************************************
 *  Copyright (C) 2001-2002 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Andrew Uselton (uselton2@llnl.gov>
 *  UCRL-CODE-2002-008.
 *  
 *  This file is part of PowerMan, a remote power management program.
 *  For details, see <http://www.llnl.gov/linux/powerman/>.
 *  
 *  PowerMan is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *  
 *  PowerMan is distributed in the hope that it will be useful, but WITHOUT 
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
 *  for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with PowerMan; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
\*****************************************************************************/

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "powerman.h"
#include "exit_error.h"
#include "list.h"
#include "config.h"
#include "device.h"
#include "powermand.h"
#include "action.h"
#include "wrappers.h"
#include "pm_string.h"
#include "config.h"

/*
 *   Constructor - This is a sort of degenerate Protocol specifically
 * for the client protocol.  It doesn't require the flexibility of
 * the device protocols, because I get to write the client that 
 * talks to it.  Every script has exactly two steps.  The first is 
 * recognizable string for the command in an EXPECT structure, the 
 * second is not even recorded here.  It is the hard coded response
 * the server always makes to the client in client_reply().
 *
 * Produces:  Protocol
 *            in particular the g->client_protocol
 */ 
Protocol *
init_Client_Protocol(void)
{
/* 
 * Initialize the expect/send pairs for the client protocol. 
 * Each one has exactly one EXPECT.  
 */
	Protocol *client_prot;
	List *scripts;
	Script_El *script_el;
	struct timeval tv;
	String string1;
	String string2;

	tv.tv_sec = 0;   /* These aren't used here, but the      */
	tv.tv_usec = 0;  /* make_Script_El function needs a value. */
	client_prot = (Protocol *)Malloc(sizeof(Protocol));

	scripts = (List *)Malloc(NUM_SCRIPTS*sizeof(List));
	client_prot->scripts = scripts;
	client_prot->num_scripts = NUM_SCRIPTS;
	client_prot->mode = REGEX;

/*
 *  N.B. The server.c:client_reply() function hard codes the actual
 * replies generated by each command.  The following are just the 
 * means for detecting which function the client in instigating.  
 */

/* PM_ERROR 0 */
	scripts[PM_ERROR] = NULL;

/* PM_LOG_IN 1 */
	scripts[PM_LOG_IN] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("powerman"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_LOG_IN], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_CHECK_LOGIN 2 */
	scripts[PM_CHECK_LOGIN] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("check login"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_CHECK_LOGIN], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_LOG_OUT 3 */
	scripts[PM_LOG_OUT] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("quit"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_LOG_OUT], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_UPDATE_PLUGS 4 */
	scripts[PM_UPDATE_PLUGS] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("update plugs"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_UPDATE_PLUGS], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_UPDATE_NODES 5 */
	scripts[PM_UPDATE_NODES] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("update nodes"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_UPDATE_NODES], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_POWER_ON 6 */
	scripts[PM_POWER_ON] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("on ([^[:space:]]+)"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_POWER_ON], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_POWER_OFF 7 */
	scripts[PM_POWER_OFF] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("off ([^[:space:]]+)"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_POWER_OFF], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_POWER_CYCLE 8 */
	scripts[PM_POWER_CYCLE] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("cycle ([^[:space:]]+)"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_POWER_CYCLE], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_RESET 9 */
	scripts[PM_RESET] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("reset ([^[:space:]]+)"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_RESET], script_el);
	free_String((void *)string1); free_String((void *)string2);

/* PM_NAMES 9 */
	scripts[PM_NAMES] = list_create(free_Script_El);
	script_el = make_Script_El(EXPECT, string1 = make_String("names ([^[:space:]]+)"), string2 = make_String("\n"), NULL, tv);
	list_append(scripts[PM_NAMES], script_el);
	free_String((void *)string1); free_String((void *)string2);

	return client_prot;
}


/*******************************************************************/
/*                                                                 */
/* Script_El                                                       */
/*   A Script_El structure is an element in a script.  The script  */
/* is a list of these elemments that implements one of the actions */
/* that clients may request of devices.                            */
/*                                                                 */
/*******************************************************************/

/*
 *   Constructor
 *
 * Produces:  Script_El
 */
Script_El *
make_Script_El(Script_El_T type, String s1, String s2, List map, struct timeval tv)
{
	Script_El *script_el;
	reg_syntax_t cflags = REG_EXTENDED;

	script_el = (Script_El *)Malloc(sizeof(Script_El));
	script_el->type = type;
	switch(type)
	{
	case SEND :
		script_el->s_or_e.send.fmt = copy_String(s1);
		break;
	case EXPECT :
		/* XXX jg - use get_String accessor instead of s->string, 
		 * but is this correct in the first place? */
		Regcomp( &(script_el->s_or_e.expect.completion), get_String(s2), cflags);
		/* XXX jg see line above */
		Regcomp( &(script_el->s_or_e.expect.exp), get_String(s1), cflags);
		script_el->s_or_e.expect.map = map;
		break;
	case DELAY :
		script_el->s_or_e.delay.tv = tv;		
		break;
	default :
	}
	return script_el;
}

#ifndef NDUMP

/*
 *  Debug structure dump routine 
 */
void
dump_Script(List script, int num)
{
	ListIterator script_itr;
	Script_El *script_el;

	fprintf(stderr, "\t\tScript %s: %0x\n", 
		pm_coms[num], (unsigned int)script);
	script_itr = list_iterator_create(script);
	while( (script_el = list_next(script_itr)) )
	{
		fprintf(stderr, "\t\t\ttype: ");
		switch( script_el->type )
		{
		case SND_EXP_UNKNOWN :
			fprintf(stderr, "SND_EXP_UNKNOWN\n");
			break;
		case SEND :
			fprintf(stderr, "SEND\n");
			dump_Send(&(script_el->s_or_e.send));
			break;
		case EXPECT :
			fprintf(stderr, "EXPECT\n");
			dump_Expect(&(script_el->s_or_e.expect));
			break;
		case DELAY :
			fprintf(stderr, "DEALY\n");
			/* FIXME: def of s_or_e.delay has changed? */
			/*fprintf(stderr, "\t\t\t%d.%06d\n", script_el->s_or_e.delay.tv_sec, script_el->s_or_e.delay.tv_usec);*/
			break;
		default :
			fprintf(stderr, "unknown\n");
		}
	}
	list_iterator_destroy(script_itr);
}

/*
 *  Debug structure dump routine 
 */
void
dump_Send(Send_T *send)
{
	fprintf(stderr, "\t\t\tSend: %0x\n", (unsigned int)send);
	fprintf(stderr, "\t\t\t\tfmt: %s\n", get_String(send->fmt));
}

/*
 *  Debug structure dump routine 
 */
void
dump_Expect(Expect_T *expect)
{
	Interpretation *interp;
	ListIterator map_i;

	fprintf(stderr, "\t\t\tExpect: %0x\n", (unsigned int)expect);
	if(expect->map == NULL) fprintf(stderr, "\t\t\t\tmap: NULL\n");
	else 
	{
		map_i = list_iterator_create(expect->map);
		while( (interp = list_next(map_i)) )
		{
			dump_Interpretation(interp);
		}
		list_iterator_destroy(map_i);
	}
}

#endif

/*
 *   Destructor
 *
 * Destroys:  Script_El
 */
void
free_Script_El(void *script_el)
{
	assert( script_el != NULL );
	switch( ((Script_El *)script_el)->type)
	{
	case SEND :
		free_String((void *)((Script_El *)script_el)->s_or_e.send.fmt);
		break;
	case EXPECT :
		list_destroy(((Script_El *)script_el)->s_or_e.expect.map);
		break;
	case DELAY :
	default :
	}
	Free( (Script_El *)script_el );
}

/*******************************************************************/
/*                                                                 */
/* Spec                                                            */
/*    A Spec struct holds all the information about a device type  */
/* while the config file is being parsed.  Device lines refer to   */
/* entries in the Spec list to get their detailed construction     */
/* and configuration information.  The Spec is not used after the  */
/* config file is closed.                                          */
/*                                                                 */
/*******************************************************************/
/*
 *   Constructor
 *
 * Produces:  Spec
 */
Spec *
make_Spec(char *name)
{
	Spec *spec;
	int i;

	spec = (Spec *)Malloc(sizeof(Spec));
	spec->name = make_String(name);
	spec->type = NO_DEV;
	spec->num_scripts = NUM_SCRIPTS;
	spec->size = 0;
	spec->timeout.tv_sec = 0;
	spec->timeout.tv_usec = 0;
	spec->mode = NO_MODE;
	spec->scripts = (List *)Malloc(spec->num_scripts*sizeof(List));
	for (i = 0; i < spec->num_scripts; i++)
		spec->scripts[i] = NULL;
	return spec;
}

/*
 *   This match utility is compatible with the list API's ListFindF
 * prototype for searching a list of Spec * structs.  The match
 * criterion is a string match on their names.  This comes into use 
 * in the parser when the device specifications have been parsed
 * into a list and a device line referes to a device specification
 * by its name.  
 */
int
match_Spec(void *spec, void *key)
{
	if( match_String(((Spec *)spec)->name, (char *)key) )
		return TRUE;
	return FALSE;
}

#ifndef NDUMP

/*
 *  Debug structure dump routine 
 */
void
dump_Spec(Spec *spec)
{
	ListIterator script;
	Spec_El *specl;
	int i;

	fprintf(stderr, "\tSpec: %0x\n", (unsigned int)spec);
	if(spec == NULL) return;
	fprintf(stderr, "\t\tname: %s\n", get_String(spec->name));
	fprintf(stderr, "\t\tspec type: ");
	if(spec->type == NO_DEV) fprintf(stderr, "NO_DEV\n");
	else if(spec->type == TTY_DEV)
	{
		fprintf(stderr, "TTY_DEV\n");
	}
	else if(spec->type == TCP_DEV)
	{
		fprintf(stderr, "TCP_DEV\n");
	}
	else if(spec->type == PMD_DEV)
	{
		fprintf(stderr, "PMD_DEV\n");
	}
	else if(spec->type == TELNET_DEV)
	{
		fprintf(stderr, "TELNET_DEV\n");
	}
	else if(spec->type == SNMP_DEV)
	{
		fprintf(stderr, "SNMP_DEV\n");
	}
	else fprintf(stderr, "unknown\n");
	fprintf(stderr, "\t\toff value: %s\n", get_String(spec->off));
	fprintf(stderr, "\t\ton value: %s\n", get_String(spec->on));
	fprintf(stderr, "\t\tall value: %s\n", get_String(spec->all));
	fprintf(stderr, "\t\tsize: %d\n", spec->size);
	for (i = 0; i < spec->num_scripts; i++)
	{
		fprintf(stderr, "\t\t%s script:\n", pm_coms[i]);
		script = list_iterator_create(spec->scripts[i]);
		while( (specl = list_next(script)) )
			dump_Spec_El(specl);
	}
}

#endif

void
free_Spec(void *vspec)
{
	int i;
	Spec *spec = (Spec *)vspec;

	free_String((void *)spec->name);  spec->name = NULL;
	free_String((void *)spec->off);   spec->off  = NULL;
	free_String((void *)spec->on);    spec->on   = NULL;
	free_String((void *)spec->all);   spec->all  = NULL;
	if( spec->type != PMD_DEV )
	{
		for (i = 0; i < spec->size; i++)
		{
			free_String((void *)spec->plugname[i]);
		}
		Free(spec->plugname);
	}
	for (i = 0; i < spec->num_scripts; i++)
		list_destroy(spec->scripts[i]);
	Free(spec->scripts);
	Free(spec);
}

/*******************************************************************/
/*                                                                 */
/* Spec_El                                                         */
/*   A Spec_El structure is an object associated with a Spec       */
/* that holds information for a Script_El.  The Script_El itself   */
/* cannot be constructed unitl it is added to the device's         */
/* protocol.  All the infromation the Script_El needs at that time */
/* is present in the Spec_El.                                      */
/*                                                                 */
/*******************************************************************/
/*
 *   Constructor
 *
 * Produces:  Spec_El
 */
Spec_El *
make_Spec_El(Script_El_T type, char *str1, char *str2, List map)
{
	Spec_El *specl;

	specl = (Spec_El *)Malloc(sizeof(Spec_El));
	specl->type = type;
	switch(type)
	{
	case SEND :
	case EXPECT :
		specl->tv.tv_sec = 0;
		specl->tv.tv_usec = 0;
		break;
	case DELAY :
		set_tv( &(specl->tv), str1);
		break;
	default :
		assert( FALSE );
	}
	specl->string1 = make_String(str1);
	specl->string2 = make_String(str2);
	specl->map = map;
	return specl;
}

#ifndef NDUMP

/*
 *  Debug structure dump routine 
 */
void
dump_Spec_El(Spec_El *specl)
{
	fprintf(stderr, "\t\t\tSpec_El: %0x\n", (unsigned int)specl);
	switch (specl->type)
	{
	case SEND:
		fprintf(stderr, "\t\t\t\ttype:  SEND\n");
		fprintf(stderr, "\t\t\t\tstring1:  %s\n", get_String(specl->string1));
		break;
	case  EXPECT : 
		fprintf(stderr, "\t\t\t\ttype:  EXPECT\n");
		fprintf(stderr, "\t\t\t\tstring1:  %s\n", get_String(specl->string1));
		fprintf(stderr, "\t\t\t\tstring2:  %s\n", get_String(specl->string2));
		break;
	case  DELAY : 
		fprintf(stderr, "\t\t\t\ttype:  DEALY\n");
		break;
	case SND_EXP_UNKNOWN:
		break;
	}
}

#endif


void
free_Spec_El(void *specl)
{
	assert( ((Spec_El *)specl)->string1 != NULL );
	assert( ((Spec_El *)specl)->string2 != NULL );
	free_String( (void *)((Spec_El *)specl)->string1 );
	((Spec_El *)specl)->string1 = NULL;
	free_String( (void *)((Spec_El *)specl)->string2 );
	((Spec_El *)specl)->string2 = NULL;
	list_destroy(((Spec_El *)specl)->map);
	((Spec_El *)specl)->map = NULL;
	Free(specl);
}


/*******************************************************************/
/*                                                                 */
/* Cluster                                                        */
/*   A Cluster structure holds the list of Nodes in the cluster    */
/* along with how many there are and when their state was last     */
/* updated.                                                        */
/*                                                                 */
/*******************************************************************/
/*
 *   Constructor
 *
 * Produces:  Cluster
 */
Cluster *
make_Cluster(void)
{
	Cluster *cluster;

	cluster = (Cluster *)Malloc(sizeof(Cluster));
	cluster->update_interval.tv_sec  = UPDATE_SECONDS;
	cluster->update_interval.tv_usec = 0;
	cluster->num = 0;
	cluster->nodes = list_create(free_Node);
	Gettimeofday( &(cluster->time_stamp), NULL);
	return cluster;
}


#ifndef NDUMP

/*
 *  Debug structure dump routine 
 */
void
dump_Cluster(Cluster *cluster)
{
	Node *node;
	ListIterator node_i;

	fprintf(stderr, "\tCluster:%0x\n", (unsigned int)cluster);
	fprintf(stderr, "\t\tNumber of nodes: %d\n", cluster->num);
	node_i = list_iterator_create(cluster->nodes);
	while( (node = list_next(node_i)) ) 
		dump_Node(node);
	list_iterator_destroy(node_i);
	fprintf(stderr, "\t\tTime stamp: %s", ctime(&(cluster->time_stamp.tv_sec)));
}

#endif

void
free_Cluster(void *cluster)
{
	list_destroy(((Cluster *)cluster)->nodes);
	Free(cluster);
}

/*******************************************************************/
/*                                                                 */
/* Node                                                            */
/*   A Node structure holds the information for a single computer  */
/* in the cluster.  It has a name and hard- and soft-power state   */
/* as well as pointers indication with Device struct(s) are        */
/* responsible for managing this Node.                             */
/*                                                                 */
/*******************************************************************/
/*
 *   Constructor
 *
 * Produces:  Node
 */
Node *
make_Node(const char *name)
{
	Node *node;


	node = (Node *)Malloc(sizeof(Node));
	INIT_MAGIC(node);
	node->name = make_String( name );
	node->p_state = ST_UNKNOWN;
	node->p_dev = NULL;
	node->p_index = NOT_SET;
	node->n_state = ST_UNKNOWN;
	node->n_dev = NULL;
	node->n_index = NOT_SET;
	return node;
}	

int
match_Node(void *node, void *key)
{
	if( match_String(((Node *)node)->name, (char *)key) )
		return TRUE;
	return FALSE;	
}
#ifndef NDUMP

/*
 *  Debug structure dump routine 
 */
void
dump_Node(Node *node)
{
	fprintf(stderr, "\t\tNode: %0x\n", (unsigned int)node);
	fprintf(stderr, "\t\t\tname: %s\n", get_String(node->name));
	if(node->p_dev != NULL)
	{
		fprintf(stderr, "\t\t\thard power state: ");
		if(node->p_state == ST_UNKNOWN) fprintf(stderr, "ST_UNKNOWN\n");
		else if(node->p_state == OFF) fprintf(stderr, "OFF\n");
		else if(node->p_state == ON) fprintf(stderr, "ON\n");
		else fprintf(stderr, "unknown\n");
		fprintf(stderr, "\t\t\tdevice: %s\n", get_String(node->p_dev->name));
		fprintf(stderr, "\t\t\tindex: %d\n", node->p_index);
	}
	else
		fprintf(stderr, "\t\t\tNo hard power state available\n");
	if(node->n_dev != NULL)
	{
		fprintf(stderr, "\t\t\tsoft power state: ");
		if(node->n_state == ST_UNKNOWN) fprintf(stderr, "ST_UNKNOWN\n");
		else if(node->n_state == OFF) fprintf(stderr, "OFF\n");
		else if(node->n_state == ON) fprintf(stderr, "ON\n");
		else fprintf(stderr, "unknown\n");
		fprintf(stderr, "\t\t\tdevice: %s\n", get_String(node->n_dev->name));
		fprintf(stderr, "\t\t\tindex: %d\n", node->n_index);
	}
	else
		fprintf(stderr, "\t\t\tNo soft power state available\n");
}

#endif

void
free_Node(void *node)
{
	free_String( (void *)((Node *)node)->name);
	Free(node);
}

/*******************************************************************/
/*                                                                 */
/* Interpretation                                                  */
/*   An Interpretation structure is the means by which an EXPECT   */
/* script element identifies the pieces of the RegEx matched in an */
/* may be interpreted to give state information.                   */
/*                                                                 */
/*******************************************************************/
/*
 *   Constructor
 *
 * Produces:  Interpretation
 */
Interpretation *
make_Interp(char *name)
{
	Interpretation *interp;
	
	interp = (Interpretation *)Malloc(sizeof(Interpretation));
	interp->plug_name = make_String(name);
	interp->match_pos = NOT_SET;
	interp->val = NULL;
	interp->node = NULL;
	return interp;
}

int
match_Interp(void *interp, void *key)
{
	if( match_String(((Interpretation *)interp)->plug_name, (char *)key) )
		return TRUE;
	return FALSE;	
}

#ifndef NDUMP


/*
 *  Debug structure dump routine 
 */
void
dump_Interpretation(Interpretation *interp)
{
	fprintf(stderr, "\t\t\t\tInterpretation: %0x\n", (unsigned int)interp);
	fprintf(stderr, "\t\t\t\t\tMatch Position: %d\n", interp->match_pos);
	if(interp->val == NULL) fprintf(stderr, "\t\t\t\t\tval: NULL\n");
	else fprintf(stderr, "\t\t\t\t\tval: %s\n", interp->val);
	fprintf(stderr, "\t\t\t\t\tnode: %s\n", get_String(interp->node->name));
}

#endif

void
free_Interp(void *interp)
{
	free_String( (void *)((Interpretation *)interp)->plug_name);
	Free(interp);
}

/*
 *  During parsing struct timeval values in the config file will be
 * entered as decimals.  The lexer picks out the string and this 
 * function translates that arbitrary precision string into the 
 * seconds, micro-seconds form of a struct timeval.  Right now
 * it insists there be an integer,  decimal, integer.  I could 
 * improve the thing by allowing a variety of formats: 
 * 10 10.10 0.10 .10 
 */
#define TMPSTRLEN 32
void
set_tv(struct timeval *tv, char *s)
{
	int len;
	char decimal[TMPSTRLEN];
	int sec;
	int usec;
	int n;

	assert( tv != NULL );
	n = sscanf(s, "%d.%s", &sec, decimal);
	if( (n != 2) || (sec < 0) ) 
		exit_msg("couldn't get the seconds and useconds from %s", s);
	len = strlen(decimal);
	n = sscanf(decimal, "%d", &usec);
	if( (n != 1) || (usec < 0) ) 
		exit_msg("couldn't get the useconds from %s", decimal);
	while( len > 6 )
	{
		usec /= 10;
		len--;
	}
	while( len < 6 )
	{
		usec *= 10;
		len++;
	}
	tv->tv_sec  = sec;
	tv->tv_usec = usec;
}
