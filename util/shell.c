/*
 * Copyright (c) 2010 Quantenna Communications, Inc.
 * All rights reserved.
 *
 *  Shell app
 */

// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////
#include <cmdo.h>
#include <stdio.h>
#include <string.h>
#include <shell.h>
#include <version.h>
#ifdef ENABLE_ENV
#include <env.h>
#endif
#ifdef ENABLE_SYMBOL
struct shell_symbol symbol_storage[SHELL_MAX_SYMBOLS];
TAILQ_HEAD(symbol_cache, shell_symbol) symbol_cache;
TAILQ_HEAD(symbol_pool, shell_symbol) symbol_pool;
#endif
#ifndef SIM
// /////////////////////////////////////////////////////////////////////////////
// Data
// /////////////////////////////////////////////////////////////////////////////
u32 g_shell_result = 0;

void banner(void)
{
	int rev = REG_READ(SYS_CSR);
	extern char *build_str;
#ifdef CMD_TEMP
	u32 temp_min,temp_max;
#endif
	printf("\n\n");
	printf("   ____ _         __ _          _ _ \n");
	printf("  /___ \\ |_ _ __ / _\\ |__   ___| | |\n");
	printf(" //  / / __| '_ \\\\ \\| '_ \\ / _ \\ | |\n");
	printf("/ \\_/ /| |_| | | |\\ \\ | | |  __/ | |\n");
	printf("\\___,_\\ \\__|_| |_\\__/_| |_|\\___|_|_|\n");
#if (BBIC_VER < 0x40)
	printf("\nBBIC4 Device Rev %x.%x\n",(rev>>4) & 0xf,(rev) & 0xf);
#else
	printf("\nPearl BBIC5 Device Rev %x.%x\n",(rev>>4) & 0xf,(rev) & 0xf);
#endif
	printf("LH,MUC: %d MHz, DSP: %d MHz, AUC: %d MHz, DDR %d MHz\n",
		cpu_lhost_freq(),cpu_dsp_freq(),cpu_auc_freq(),cpu_ddr_freq());
	printf("%s\n",build_str);
#ifdef CMD_TEMP
	temp_get(&temp_min,&temp_max);
	printf("BBIC Temperature : %dC - %dC\n", temp_min, temp_max);
#endif
	printf("\n");
}

/******************************************************************************
   Function:    shell_find_cmd
   Purpose:    	Find shell command
   Returns:
   Note:    
 *****************************************************************************/
shell_cmd_t *shell_find_cmd (const char *cmd,shell_cmd_t *start,shell_cmd_t *stop)
{
	shell_cmd_t *cmd_ptr;
	shell_cmd_t *cmd_ptr_tmp = start;	/*Init value */
	const char *p;
	int len;
	int n_found = 0;

	/*
	 * Some commands allow length modifiers (like "cp.b");
	 * compare command name only until first dot.
	 */
	len = ((p = strchr(cmd, '.')) == NULL) ? strlen (cmd) : (p - cmd);

	for (cmd_ptr = start; cmd_ptr != stop; cmd_ptr++) {
		if (strncmp (cmd, cmd_ptr->name, len) == 0) {
			
			//printf("%s %s %d %d\n",cmd, cmd_ptr->name, len,strlen (cmd_ptr->name));
			if (len == strlen (cmd_ptr->name))
				return cmd_ptr;	/* full match */

			cmd_ptr_tmp = cmd_ptr;	/* abbreviated command ? */
			n_found++;
		}
	}
	if (n_found == 1) {			/* exactly one match */
		return cmd_ptr_tmp;
	}

	return NULL;	/* not found or ambiguous command */
}
//static int g_num_cmds = sizeof(g_shell_cmd) / sizeof(shell_cmd_t);

/******************************************************************************
   Function:	shell_help
   Purpose:	Get	char via uart.	This is	low	level interface	for	stdin
   Returns:
   Note:	
 *****************************************************************************/
int shell_parse(struct shell_cmd *cmdtable, int nCmds, char *str)
{
	char len;
	int i;
	int ret = -1;

	// calculate len of cmd
	len = 0;
	while ((str[len] != ' ') && (str[len] != 0)
	       && (str[len] != '\n') && (str[len] != '\r')) {
		len++;
	}

	// process the command
	for (i = 0; i < nCmds; i++) {
		if (strncmp(cmdtable[i].name, str, len) == 0) {
			ret = cmdtable[i].fn(str);
			if (ret < 0) {
				printf("Usage: %s\n", cmdtable[i].help);
			}
			i = nCmds;
		}
	}
	return ret;
}

char user_prompt[16]="";

int change_prompt(char *cmd)
{
	char *token;
	token = strtok(cmd," \n\r");
	if (token == NULL) {
		return -1;
	}
	token = strtok(NULL,"\n\r");
	if (token == NULL) {
		return -1;
	}
	strcpy(user_prompt,token);
	return 0;
}
SHELL_CMD(prompt,change_prompt,"change prompt","prompt <new prompt>");

static char g_prompt[9];
void prompt(void)
{
	printf(g_prompt);
}

extern u32 console_owner;
/******************************************************************************
   Function:	shell_help
   Purpose:	Get	char via uart.	This is	low	level interface	for	stdin
   Returns:
   Note:	
 *****************************************************************************/
void shell(void)
{
	char *str = 0;
	char buffer[255];
	int init = 0;

#ifdef ENABLE_SYMBOL
	TAILQ_INIT(&symbol_cache);
	TAILQ_INIT(&symbol_pool);
	int i;
	for (i=0;i<SHELL_MAX_SYMBOLS;i++) {
		TAILQ_INSERT_HEAD(&symbol_pool,&symbol_storage[i],entries);
	}
#endif

#ifdef CPU_MUC
	sprintf(g_prompt,"muc >");
#elif defined(CPU_LH)
	sprintf(g_prompt,"lhost >");
#else
	sprintf(g_prompt,"dsp >");
#endif

	while (REG_READ(&console_owner) != WHICH_CPU) {
	}
	banner();
	while (1) {
		while (REG_READ(&console_owner) != WHICH_CPU) {
		}
		if (strlen(user_prompt) > 0) {
			str = rxline(user_prompt,buffer);
		} else {
			str = rxline(g_prompt,buffer);
		}
		shell_rpc(str);	
	}
}

void shell_rpc(char *str_in)
{
	char buffer1[255];
	char command[255];
	char buffer[255];
	struct shell_cmd *cmd;
#ifdef ENABLE_ENV	
	char *env;
#endif	
	char *token,*next;
	char *str = str_in;
	strcpy(buffer,str);
	if (strlen(str) > 0) {
		if ((strlen(str) == 1) && *str == '?') {
			strcpy(str,"help");
		}
		// use new method
		strcpy(command,buffer);
		str = strtok(command," \t\n\r,");
		if (str[0] == '#') {
			// comment
		} else if ((strstr(buffer,"env") == NULL) && (strchr(buffer,';') != NULL)) {
			// sequence of commands
			strcpy(buffer1,buffer);		// need this to preserve env text
			token = _strtok_r(buffer1,";",&next);
			while (token != NULL) {
				printf("env>%s\n",token);
				strcpy(buffer,token); // need this to preserve cmd line
				strcpy(command,token);  // process command in this buffer
				str = strtok(command," \t\n\r,");
				if ((cmd = shell_find_cmd(str,&__command_start,&__command_end)) 
										!= NULL) {
					if (cmd->fn(buffer) < 0) {
						printf("Usage: %s\n",cmd->usage);
					}
				}
				token = _strtok_r(NULL,";",&next);
			}
		} else if ((cmd = shell_find_cmd(str,&__command_start,&__command_end)) != NULL) {
			if (cmd->fn(buffer) < 0) {
				printf("Usage: %s\n",cmd->usage);
			}
		} 
#ifdef ENABLE_ENV	
		else if ((env = env_get(str)) != NULL) {
			// at this point we have string of commands separated by ';'
			strcpy(buffer1,env);		// need this to preserve env text
			token = _strtok_r(buffer1,";",&next);
			while (token != NULL) {
				printf("env>%s\n",token);
				strcpy(buffer,token); // need this to preserve cmd line
				strcpy(command,token);  // process command in this buffer
				str = strtok(command," \t\n\r,");
				if (str[0] != '#') {
					// ignore comments
					if ((cmd = shell_find_cmd(str,&__command_start,&__command_end)) 
											!= NULL) {
						if (cmd->fn(buffer) < 0) {
							printf("Usage: %s\n",cmd->usage);
						}
					}
				}
				token = _strtok_r(NULL,";",&next);
			}
		} 
#endif	
		else {
			printf("Error: command not recognized %s\n",str);
		}
		//shell_parse(g_shell_cmd, g_num_cmds, str);
	}
}

/******************************************************************************
   Function:	shell_help
   Purpose:	print help text
   Returns:
   Note:	
 *****************************************************************************/
int shell_help(char *)
{
	shell_cmd_t *cmd_ptr;
	banner();
	for (cmd_ptr = &__command_start; cmd_ptr != &__command_end; cmd_ptr++) {
		if (strlen(cmd_ptr->help) > 0) {
			printf("%-10s %-20s - %s\n",cmd_ptr->name, cmd_ptr->help,cmd_ptr->usage);
		}
	}
	return 0;
}
SHELL_CMD(help,shell_help,"Display help","help");

#endif // ifndef SIM

/******************************************************************************
   Function:    getintArg
   Purpose:     get unsigned integer argument from string
   Returns:
   Note:    
 *****************************************************************************/
u32 get_int_arg(char *buffer)
{
	u32 value = SRAM_BASE_ADDR;
#ifdef ENABLE_ENV	
	char *str;
#endif
#ifdef ENABLE_SYMBOL
	// symbol cache is runtime, env cache is not
	if (buffer[0] == '$') {
		struct shell_symbol *ptr = shell_get_symbol(buffer);
		if (ptr == NULL) {
			printf("Error: symbol not defined %s\n",buffer);
		} else {
			value = ptr->value;
		}
	} else 
#endif		
	if (strncmp(buffer, "0x", 2) == 0) {
		sscanf(buffer, "0x%x", &value);
	} else if (buffer[0] >= '0' && buffer[0] <= '9') {
		sscanf(buffer, "%d", &value);
	} 
#ifdef ENABLE_ENV	
	else if ((str = env_get(buffer)) != NULL) {
		// recursive call ---
		value = get_int_arg(str);
	}
#endif
	return value;
}

/******************************************************************************
   Function:    get_hex_arg
   Purpose:     get unsigned integer argument from string
   Returns:
   Note:    
 *****************************************************************************/
u32 get_hex_arg(char *buffer)
{
	u32 value = SRAM_BASE_ADDR;
#ifdef ENABLE_ENV	
	char *str;
#endif
#ifdef ENABLE_SYMBOL
	// symbol cache is runtime, env cache is not
	if (buffer[0] == '$') {
		struct shell_symbol *ptr = shell_get_symbol(buffer);
		if (ptr == NULL) {
			printf("Error: symbol not defined %s\n",buffer);
		} else {
			value = ptr->value;
		}
	} else 
#endif		
	if (sscanf(buffer, "%x", &value) == 1) {
		return value;
	} 
#ifdef ENABLE_ENV	
	else if ((str = env_get(buffer)) != NULL) {
		// recursive call ---
		value = get_hex_arg(str);
	}
#endif
	return value;
}

/**************************************************************************
    Function:	shell_parse_opts
	        Option processing algorithm
    Purpose:	Pass structure to command options
    		The structure will be filled in with 
    Returns:	
    Note:	
 *************************************************************************/
int shell_parse_opts(char *arg, struct opt_table *options)
{
	char *token;
	int cmd = -1;
	int i,j;
	token = strtok(arg, " ");	// command - ignore
	token = strtok(NULL, " ");
	while (token != NULL) {
		i=0;
		while (options[i].option != NULL) {
			if (strcmp(options[i].option,"default") == 0) {
				// we allow int arg or hex arg here
				if (options[i].flags == OPTION_FLAG_INT_VAR) {
					// check for valid int
					u32 val;
					if ((token[0] == '0') && token[1] == 'x') {
						*(u32 *)options[i].value = get_hex_arg(token);
						break;
					} else if (sscanf(token,"%d",&val) == 1) {
						(u32)options[i].value = val;
						break;
					}
				} else if (options[i].flags == OPTION_FLAG_HEX_VAR) {
					*(u32 *)options[i].value = get_hex_arg(token);
				}
			} else if (strcmp(token,options[i].option) == 0) {
				if (options[i].flags == OPTION_FLAG_FUNC) {
					shell_cb_t fn = (shell_cb_t)options[i].value;
					fn((void *)options[i].str_opt);
					break;
				} else if (options[i].flags == OPTION_FLAG_COMMAND) {
					cmd = (int)options[i].value;
					break;
				} else if (options[i].flags == OPTION_FLAG_SWITCH) {
					*(u32 *)options[i].value = !*(u32 *)options[i].value;
					if (cmd == -1) {
						cmd = 0;
					}
					break;
				} else if (options[i].flags == OPTION_FLAG_INT_VAR) {
					if ((token = strtok(NULL, " ")) ==  NULL) {
						return 0;
					}
					*(u32 *)options[i].value = get_int_arg(token);
					if (cmd == -1) {
						cmd = 0;
					}
					break;
				} else if (options[i].flags == OPTION_FLAG_HEX_VAR) {
					if ((token = strtok(NULL, " ")) ==  NULL) {
						return 0;
					}
					*(u32 *)options[i].value = get_hex_arg(token);
					if (cmd == -1) {
						cmd = 0;
					}
					break;
				} else if (options[i].flags == OPTION_FLAG_MAC_VAR) {
					int k;
					u8 *ptr = (u8 *)options[i].value;
					for (k=0;k<6;k++) {
						token = strtok(NULL, " ,:.");
						if (token == NULL) {
							return -1;
						}
						ptr[k] = (u8)get_hex_arg(token);
					}
					if (cmd == -1) {
						cmd = 0;
					}
					break;
				} else if (options[i].flags == OPTION_FLAG_IP_VAR) {
					int k;
					u8 *ptr = (u8 *)options[i].value;
					for (k=0;k<4;k++) {
						token = strtok(NULL, " ,.:");
						if (token == NULL) {
							return -1;
						}
						ptr[k] = (u8)get_int_arg(token);
					}
					if (cmd == -1) {
						cmd = 0;
					}
					break;
				} else if (options[i].flags == OPTION_FLAG_STR_VAR) {
					strcpy(options[i].value,token);
				} else if (options[i].flags == OPTION_FLAG_STR_CMP) {
					if ((token = strtok(NULL, " ")) ==  NULL) {
						return 0;
					}
					// searc string table for this option
					j = 0;
					while (options[i].str_opt[j].str_opt != NULL) {
						if (strcmp(token,options[i].str_opt[j].str_opt) == 0) {
							*(u32 *)options[i].value = options[i].str_opt[j].value;
							break;
						}
						j++;
					}
					if (cmd == -1) {
						cmd = 0;
					}
					break;
				} 
			}

			i++;
		}
		token = strtok(NULL, " ");
	}
	return cmd;
}

#ifdef ENABLE_SYMBOL
/**************************************************************************
    Function:	shell_get_symbol
    Purpose:	Get symbol value
    Returns:	
    Note:	May need to hash if the table gets too big
 *************************************************************************/
struct shell_symbol * 
shell_get_symbol(char *arg)
{
	struct shell_symbol *ptr = (struct shell_symbol *)TAILQ_FIRST(&symbol_cache);

	while (ptr != NULL) {
		if (strcmp(ptr->name,arg) == 0) {
			return ptr;
		}
		ptr = TAILQ_NEXT(ptr,entries);
	}
	return NULL;
}

/**************************************************************************
    Function:	shell_list_symbol
    Purpose:	list symbols and values
    Returns:	
    Note:	
 *************************************************************************/
int shell_list_symbol(char *arg)
{
	struct shell_symbol *ptr = (struct shell_symbol *)TAILQ_FIRST(&symbol_cache);

	while (ptr != NULL) {
		printf("%-40s %08x\n",ptr->name,ptr->value);
		ptr = TAILQ_NEXT(ptr,entries);
	}
	return 0;
}

/**************************************************************************
    Function:	shell_get_symbol_cmd
    Purpose:	Shell command interface for get symbol
    Returns:	
    Note:	
 *************************************************************************/
int shell_get_symbol_cmd(char *arg)
{
	char *token;
	struct shell_symbol *ptr;
	token = strtok(arg, " ");	// command - ignore
	token = strtok(NULL, " ");
	if ((ptr = shell_get_symbol(token)) != NULL) {
		printf("%s -> %x\n",ptr->name,ptr->value);
	}
	return 0;
}

/**************************************************************************
    Function:	shell_set_var
    Purpose:	Set variable	
    Returns:	
    Note:	
 *************************************************************************/
int shell_set_symbol(char *arg)
{
	char *token,*value;
	struct shell_symbol *ptr;
	token = strtok(arg, " ");	// command - ignore
	token = strtok(NULL, " ");
	value = strtok(NULL, " ");
	if ((token[0] != '$') || (strlen(token) < 2) || (strlen(token) > SHELL_SYMBOL_MAX_LEN)) {
		printf("Error: symbols must begin with $ and have valid name and be less than %d bytes\n",
				SHELL_SYMBOL_MAX_LEN);
	} else 
	if (shell_get_symbol(token) != NULL) {
		printf("Error: symbol already defined\n");
	} else {
		ptr = (struct shell_symbol *)TAILQ_FIRST(&symbol_pool);
		if (ptr == NULL) {
			printf("Error: out of symbols\n");
		} else {
			TAILQ_REMOVE(&symbol_pool,ptr,entries);
			strcpy(ptr->name,token);
			ptr->value = get_int_arg(value);
			TAILQ_INSERT_HEAD(&symbol_cache,ptr,entries);
		}
	}

	return 0;
}
SHELL_CMD(set,shell_set_symbol,"set shell symbol","set <$name> <value>");
SHELL_CMD(get,shell_get_symbol_cmd,"get shell symbol","get <$name>");
SHELL_CMD(list,shell_list_symbol,"list shell symbols","list");
#endif
