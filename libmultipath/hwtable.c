#include <stdio.h>

#include "checkers.h"
#include "vector.h"
#include "defaults.h"
#include "structs.h"
#include "config.h"
#include "pgpolicies.h"
#include "prio.h"

/*
 * Tuning suggestions on these parameters should go to
 * dm-devel@redhat.com
 * 
 * You are welcome to claim maintainership over a controller
 * family. Please mail the currently enlisted maintainer and
 * the upstream package maintainer.
 */
static struct hwentry default_hw[] = {
	/*
	 * Compellent Technologies, Inc.
	 *
	 * Maintainer : Jim Lester, Compellent
	 * Mail : jim.lester@compellent.com
	 */
	{
		.vendor        = "COMPELNT",
		.product       = "Compellent Vol",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	/*
	 * Apple controller family
	 *
	 * Maintainer : Shyam Sundar
	 * Mail : g.shyamsundar@yahoo.co.in
	 */
	{
		.vendor        = "APPLE*",
		.product       = "Xserve RAID ",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DEFAULT_CHECKER,
		.prio_name     = DEFAULT_PRIO,
	},
	/*
	 * StorageWorks controller family
	 *
	 * Maintainer : Christophe Varoqui
	 * Mail : christophe.varoqui@free.fr
	 */
	{
		.vendor        = "3PARdata",
		.product       = "VV",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DEFAULT_CHECKER,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		.vendor        = "DEC",
		.product       = "HSG80",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 hp_sw",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = HP_SW,
		.prio_name     = PRIO_HP_SW,
	},
	{
		.vendor        = "HP",
		.product       = "A6189A",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* MSA 1000/MSA1500 EVA 3000/5000 with old firmware */
		.vendor        = "(COMPAQ|HP)",
		.product       = "(MSA|HSV)1.0.*",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 hp_sw",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.checker_name  = HP_SW,
		.prio_name     = PRIO_HP_SW,
	},
	{
		/* MSA 1000/1500 with new firmware */
		.vendor        = "(COMPAQ|HP)",
		.product       = "MSA VOLUME",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},
	{
		/* EVA 3000/5000 with new firmware, EVA 4000/6000/8000 */
		.vendor        = "(COMPAQ|HP)",
		.product       = "HSV1[01]1|HSV2[01]0|HSV3[046]0|HSV4[05]0",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},
	{
		/* HP MSA2000 product family with old firmware */
		.vendor        = "HP",
		.product       = "MSA2[02]12fc|MSA2012i",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 18,
		.minio         = 100,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* HP MSA2000 product family with new firmware */
		.vendor        = "HP",
		.product       = "MSA2012sa|MSA23(12|24)(fc|i|sa)|MSA2000s VOLUME",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 18,
		.minio         = 100,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},
	{
		/* HP P2000 product family */
		.vendor        = "HP",
		.product       = "P2000",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 18,
		.minio         = 100,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},
	{
		/* HP SVSP */
		.vendor        = "HP",
		.product       = "HSVX700",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 alua",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = 100,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},

	{
		/* HP Smart Array */
		.vendor        = "HP",
		.product       = "LOGICAL VOLUME.*",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 12,
		.minio         = DEFAULT_MINIO,
		.checker_name  = CCISS_TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	/*
	 * DDN controller family
	 *
	 * Maintainer : Christophe Varoqui
	 * Mail : christophe.varoqui@free.fr
	 */
	{
		.vendor        = "DDN",
		.product       = "SAN DataDirector",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	/*
	 * EMC / Clariion controller family
	 *
	 * Maintainer : Edward Goggin, EMC
	 * Mail : egoggin@emc.com
	 */
	{
		.vendor        = "EMC",
		.product       = "SYMMETRIX",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 6,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		.vendor        = "DGC",
		.product       = ".*",
		.bl_product    = "LUNZ",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 emc",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = (300 / DEFAULT_CHECKINT),
		.minio         = DEFAULT_MINIO,
		.checker_name  = EMC_CLARIION,
		.prio_name     = PRIO_EMC,
	},
	{
		.vendor        = "EMC",
		.product       = "Invista",
		.bl_product    = "LUNZ",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = FAILOVER,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 5,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	/*
	 * Fujitsu controller family
	 *
	 * Maintainer : Christophe Varoqui
	 * Mail : christophe.varoqui@free.fr
	 */
	{
		.vendor        = "FSC",
		.product       = "CentricStor",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	/*
	 * Hitachi controller family
	 *
	 * Maintainer : Matthias Rudolph
	 * Mail : matthias.rudolph@hds.com
	 */
	{
		.vendor        = "(HITACHI|HP)",
		.product       = "OPEN-.*",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		.vendor        = "HITACHI",
		.product       = "DF.*",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_HDS,
	},
	/*
	 * IBM controller family
	 *
	 * Maintainer : Hannes Reinecke, SuSE
	 * Mail : hare@suse.de
	 */
	{
		.vendor        = "IBM",
		.product       = "ProFibre 4000R",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* IBM FAStT 1722-600 */
		.vendor        = "IBM",
		.product       = "1722-600",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 300,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		/* IBM DS4100 */
		.vendor        = "IBM",
		.product       = "1724",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 300,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		/* IBM DS3200 / DS3300 / DS3400 */
		.vendor        = "IBM",
		.product       = "1726",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 300,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		/* IBM DS4400 / DS4500 / FAStT700 */
		.vendor        = "IBM",
		.product       = "1742",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		.vendor        = "IBM",
		.product       = "174[56]",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
	    /* IBM DS4700 */
		.vendor        = "IBM",
		.product       = "1814",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
	    /* IBM DS4800 */
		.vendor        = "IBM",
		.product       = "1815",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
	    /* IBM DS5000 */
		.vendor        = "IBM",
		.product       = "1818",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		/* IBM Netfinity Fibre Channel RAID Controller Unit */
		.vendor        = "IBM",
		.product       = "3526",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		/* IBM DS4200 / FAStT200 */
		.vendor        = "IBM",
		.product       = "3542",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* IBM ESS F20 aka Shark */
		.vendor        = "IBM",
		.product       = "2105800",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* IBM ESS F20 aka Shark */
		.vendor        = "IBM",
		.product       = "2105F20",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* IBM DS6000 */
		.vendor        = "IBM",
		.product       = "1750500",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},
	{
		/* IBM DS8000 */
		.vendor        = "IBM",
		.product       = "2107900",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* IBM SAN Volume Controller */
		.vendor        = "IBM",
		.product       = "2145",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},
	{
		/* IBM S/390 ECKD DASD */
		.vendor        = "IBM",
		.product       = "S/390 DASD ECKD",
		.bl_product       = "S/390.*",
		.getuid        = "/sbin/dasdinfo -u -b %n",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* IBM S/390 FBA DASD */
		.vendor        = "IBM",
		.product       = "S/390 DASD FBA",
		.bl_product       = "S/390.*",
		.getuid        = "/sbin/dasdinfo -u -b %n",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		/* IBM IPR */
		.vendor        = "IBM",
		.product       = "IPR.*",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = "1 alua",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},
	/*
	 * IBM Power Virtual SCSI Devices
	 *
	 * Maintainer : Brian King, IBM
	 * Mail : brking@linux.vnet.ibm.com
	 */
	{
		/* AIX VDASD */
		.vendor        = "AIX",
		.product       = "VDASD",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = (300 / DEFAULT_CHECKINT),
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	/*
	 * Generic LSI/Engenio controller
	 *
	 * Maintainer: Hannes Reinecke
	 * Mail: hare@suse.de
	 */
	{
		.vendor        = "(LSI|ENGENIO)",
		.product       = "INF.*",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		/* DELL MD32xx/MD32xxi */
		.vendor        = "DELL",
		.product       = "MD32xx|MD32xxi",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		/* DELL MD3000/MD3000i */
		.vendor        = "DELL",
		.product       = "MD3000|MD3000i",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	/*
	 * NETAPP controller family
	 *
	 * Maintainer : Dave Wysochanski
	 * Mail : davidw@netapp.com
	 */
	{
		.vendor        = "NETAPP",
		.product       = "LUN.*",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = 128,
		.checker_name  = TUR,
		.prio_name     = PRIO_ONTAP,
	},
	/*
	 * NEXENTA/COMSTAR controller family
	 *
	 * Maintainer : Yacine Kheddache
	 * Mail : yacine@alyseo.com
	 */
	{
		.vendor        = "NEXENTA",
		.product       = "COMSTAR",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_SERIAL,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 30,
		.minio         = 128,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	/*
	 * IBM NSeries (NETAPP) controller family
	 *
	 * Maintainer : Dave Wysochanski
	 * Mail : davidw@netapp.com
	 */
	{
		.vendor        = "IBM",
		.product       = "Nseries.*",
		.getuid        = DEFAULT_GETUID,
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = 128,
		.checker_name  = DIRECTIO,
		.prio_name     = PRIO_ONTAP,
	},
	/*
	 * Pillar Data controller family
	 *
	 * Maintainer : Srinivasan Ramani
	 * Mail : sramani@pillardata.com
	 */
	{
		.vendor        = "Pillar",
		.product       = "Axiom.*",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
	},
	/*
	 * SGI arrays
	 *
	 * Maintainer : Christophe Varoqui
	 * Mail : christophe.varoqui@free.fr
	 */
	{
		.vendor        = "SGI",
		.product       = "TP9[13]00",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		.vendor        = "SGI",
		.product       = "TP9[45]00",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		.vendor        = "SGI",
		.product       = "IS.*",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = "2 pg_init_retries 50",
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = 15,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	/*
	 * STK arrays
	 *
	 * Maintainer : Christophe Varoqui
	 * Mail : christophe.varoqui@free.fr
	 */
	{
		.vendor        = "STK",
		.product       = "OPENstorage D280",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_RDAC,
	},
	{
		.vendor        = "STK",
		.product       = "FLEXLINE 380",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_RDAC,
	},
	/*
	 * SUN arrays
	 *
	 * Maintainer : Christophe Varoqui
	 * Mail : christophe.varoqui@free.fr
	 */
	{
		.vendor        = "SUN",
		.product       = "(StorEdge 3510|T4)",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = DIRECTIO,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		.vendor        = "SUN",
		.product       = "STK6580_6780",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_RDAC,
	},
	/*
	 * Pivot3 RAIGE
	 *
	 * Maintainer : Bart Brooks, Pivot3
	 * Mail : bartb@pivot3.com
	 */
	{
		.vendor        = "PIVOT3",
		.product       = "RAIGE VOLUME",
		.getuid        = "/lib/udev/scsi_id --page=0x80 --whitelisted --device=/dev/%n",
		.features      = "1 queue_if_no_path",
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = MULTIBUS,
		.pgfailback    = FAILBACK_UNDEF,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_UNDEF,
		.minio         = 100,
		.checker_name  = TUR,
		.prio_name     = DEFAULT_PRIO,
	},
	{
		.vendor        = "SUN",
		.product       = "CSM200_R",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	/* SUN/LSI 2510, 2540, 2530, 2540 */
	{
		.vendor        = "SUN",
		.product       = "LCSM100_[IEFS]",
		.bl_product    = "Universal Xport",
		.getuid        = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 rdac",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = RDAC,
		.prio_name     = PRIO_RDAC,
	},
	{
		.vendor	       = "Intel",
		.product       = "Multi-Flex",
		.getuid	       = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = "1 alua",
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	{
		.vendor	       = "DataCore",
		.product       = "SANmelody",
		.getuid	       = DEFAULT_GETUID,
		.features      = DEFAULT_FEATURES,
		.hwhandler     = DEFAULT_HWHANDLER,
		.selector      = DEFAULT_SELECTOR,
		.pgpolicy      = GROUP_BY_PRIO,
		.pgfailback    = -FAILBACK_IMMEDIATE,
		.rr_weight     = RR_WEIGHT_NONE,
		.no_path_retry = NO_PATH_RETRY_QUEUE,
		.minio         = DEFAULT_MINIO,
		.checker_name  = TUR,
		.prio_name     = PRIO_ALUA,
		.prio_args     = NULL,
	},
	/*
	 * EOL
	 */
	{
		.vendor        = NULL,
		.product       = NULL,
		.getuid        = NULL,
		.features      = NULL,
		.hwhandler     = NULL,
		.selector      = NULL,
		.pgpolicy      = 0,
		.pgfailback    = 0,
		.rr_weight     = 0,
		.no_path_retry = 0,
		.minio         = 0,
		.checker_name  = NULL,
		.prio_name     = NULL,
	},
};

extern int
setup_default_hwtable (vector hw)
{
	int r = 0;
	struct hwentry * hwe = default_hw;

	while (hwe->vendor) {
		r += store_hwe(hw, hwe);
		hwe++;
	}
	return r;
}
