/*
 * Copyright (c) 2009-2013 Quantenna Communications, Inc.
 * All rights reserved.
 *
 *  Syscfg module - uses config sector for common filesytem between linux and
 *  uboot.
 */
#include <queue.h>

///////////////////////////////////////////////////////////////////////////////
//             Defines
///////////////////////////////////////////////////////////////////////////////
#define OPTION_FLAG_NONE	(0)	// end of table
#define OPTION_FLAG_COMMAND	(1)	// there can only be one in a command line
#define OPTION_FLAG_SWITCH	(2)	// on or off
#define OPTION_FLAG_INT_VAR	(3)	// parse variable
#define OPTION_FLAG_HEX_VAR	(4)	// parse variable
#define OPTION_FLAG_STR_CMP	(5)	// parse variable
#define OPTION_FLAG_IP_VAR	(6)	// d.d.d.d
#define OPTION_FLAG_MAC_VAR	(7)	// x:x:x:x:x:x
#define OPTION_FLAG_STR_VAR	(8)	// string
#define OPTION_FLAG_FUNC	(9)	// function call



///////////////////////////////////////////////////////////////////////////////
//             Types
///////////////////////////////////////////////////////////////////////////////
typedef int (*shell_fn_t)(char *);
typedef void (*shell_cb_t)(void *);

typedef struct shell_cmd {
    char        *name;
    char        *help;
    char        *usage;
    shell_fn_t    fn;
} shell_cmd_t;
extern shell_cmd_t  __command_start;
extern shell_cmd_t  __command_end;

#define command_section  __attribute__ ((section (".command")))

#define SHELL_CMD(name,cmd,help,usage) \
shell_cmd_t __shell_cmd_##name command_section = {#name, help, usage, cmd}
// terminate list is NULL string ptr
struct str_opts {
	char 	*str_opt;
	u32	value;
};

// terminate list is NULL string ptr
struct opt_table {
	char		*	option;
	u32			flags;
	void		*	value;
	struct str_opts * 	str_opt;
};

#ifdef ENABLE_SYMBOL
#define SHELL_SYMBOL_MAX_LEN 48
struct shell_symbol {
	char name[SHELL_SYMBOL_MAX_LEN];
	u32 value;
	TAILQ_ENTRY(shell_symbol) entries;
};
#endif
///////////////////////////////////////////////////////////////////////////////
//             Prototypes
///////////////////////////////////////////////////////////////////////////////
void shell (void) ;

// shell commands
int shell_help(char *cmd);
int shell_md(char *cmd);
int shell_rdwr(char *cmd);
int shell_mtest8(char *cmd);
int shell_diag(char *cmd);

int shell_parse(struct shell_cmd * cmdtable,int nCmds,char *str);

/******************************************************************************
   Function:    get_int_arg
   Purpose:     get unsigned integer argument from string
   Returns:
   Note:    
 *****************************************************************************/
u32 get_int_arg(char *buffer);

/******************************************************************************
   Function:    get_hex_arg
   Purpose:     get hex integer argument from string
   Returns:
   Note:    
 *****************************************************************************/
u32 get_hex_arg(char *buffer);

/******************************************************************************
   Function:    shell_find_cmd
   Purpose:    	Find shell command
   Returns:
   Note:    
 *****************************************************************************/
shell_cmd_t *shell_find_cmd (const char *cmd,shell_cmd_t *start,shell_cmd_t *stop);

/******************************************************************************
   Function:    prompt
   Purpose:    	print prompt
   Returns:
   Note:    
 *****************************************************************************/
void prompt(void);

void shell_rpc(char *str);

/**************************************************************************
    Function:	shell_parse_opts
	        Option processing algorithm
    Purpose:	Pass structure to command options
    		The structure will be filled in with 
    Returns:	
    Note:	
 *************************************************************************/
int shell_parse_opts(char *arg, struct opt_table *options);
/**************************************************************************
    Function:	shell_set_var
    Purpose:	Set variable	
    Returns:	
    Note:	
 *************************************************************************/
int shell_set_symbol(char *arg);

/**************************************************************************
    Function:	shell_get_symbol
    Purpose:	Get symbol value
    Returns:	
    Note:	May need to hash if the table gets too big
 *************************************************************************/
struct shell_symbol * shell_get_symbol(char *arg);

