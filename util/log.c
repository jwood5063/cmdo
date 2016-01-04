/*
 * Copyright (c) 2016 Jim Wood
 * All rights reserved.
 *
 *  file purpose
 *  LOG Facility
 *
 */

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////
#include <arcshell.h>
#include <log.h>
#include <bcm2835.h>

/***************************************************************************
   Function:    
   Purpose:    
   Returns:
   Note:    
 **************************************************************************/
void log_init(struct log_dev *dev)
{
	memset(dev->entry, 0, sizeof(struct log_entry) * dev->num_entries);
	dev->rd_ptr = 0;
	dev->wr_ptr = 0;
}

void log_add(struct log_dev *dev, char *string, u32 arg1, u32 arg2)
{
	u32 wr_ptr = dev->wr_ptr;
	dev->entry[wr_ptr].timestamp = (u32)bcm2835_st_read();
	dev->entry[wr_ptr].string = string;
	dev->entry[wr_ptr].arg1 = arg1;
	dev->entry[wr_ptr].arg2 = arg2;
}

