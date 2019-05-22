// STM32   Project.                                                     GitLab
/****************************************************************************/
/*                                                                          */
/*  @file       : ring.h                                                    */
/*  @Copyright  : Powered by MULTIBEANS ORG rights reserved.                */
/*  @Revision   : Ver 1.0.                                                  */
/*  @Data       : 2018.08.26 Release.                                       */
/*  @Belong     : EDU class.                                                */
/*  @Git        :                                                           */
/*  **code : (GB2312/GBK) in Windows 10 x64. AC6            platform.       */
/****************************************************************************/
/*  @Attention:                                                             */
/*  ---------------------------------------------------------------------   */
/*  |    Data    |  Behavior |     Offer      |          Content         |  */
/*  | 2018.08.26 |   create  |Carlos Wei(M)   | ceate the document.      |  */
/*  ---------------------------------------------------------------------   */
/*  Email: carlos@mltbns.top                                  MULTIBEANS.   */
/****************************************************************************/

#ifndef RING_H_
#define RING_H_

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

typedef struct ring_t RING;

struct ring_buffer_t
{
    void*		 room;
    uint32_t     size;
	uint32_t	 max_size;
    uint32_t     in;
    uint32_t     out;
};

struct ring_master_t
{
    uint32_t (*get_current_size)(RING*);
    uint32_t (*push)(RING*, void*, uint32_t);
    uint32_t (*pop)(RING*, void*, uint32_t);
    void (*recall_back)(RING*);
};

struct ring_t
{
    struct ring_buffer_t buffer;
    struct ring_master_t *master;
};

extern RING* ring_new_dev(uint32_t ring_max_length, void* buffer_address);
extern void ring_del_dev(RING* dev);

#endif /* RING_H_ */
