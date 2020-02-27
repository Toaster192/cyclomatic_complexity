/*
 * Copyright 2011-2017 by Emese Revfy <re.emese@gmail.com>
 * Licensed under the GPL v2, or (at your option) v3
 *
 * Homepage:
 * https://github.com/ephox-gcc-plugins/cyclomatic_complexity
 *
 * http://en.wikipedia.org/wiki/Cyclomatic_complexity
 * The complexity M is then defined as:
 * M = E - N + 2P
 * where
 *
 *  E = the number of edges of the graph
 *  N = the number of nodes of the graph
 *  P = the number of connected components (exit nodes).
 *
 * Usage (4.5 - 9.2):
 * $ make clean; make run
 */

#include "gcc-common.h"

__visible int plugin_is_GPL_compatible;

FILE *log_file;

static struct plugin_info cyc_complexity_plugin_info = {
	.version	= "20170221",
	.help		= "Cyclomatic Complexity\n",
};

static unsigned int cyc_complexity_execute(void)
{
	int complexity;
	expanded_location xloc;

	/* M = E - N + 2P
	 * where P is always 1 for a single function */
	complexity = n_edges_for_fn(cfun) - n_basic_blocks_for_fn(cfun) + 2;

	xloc = expand_location(DECL_SOURCE_LOCATION(current_function_decl));
	fprintf(log_file, "%s:%d:%d:%s\t%d\n", xloc.file, xloc.line, xloc.column,
		DECL_NAME_POINTER(current_function_decl), complexity);

	return 0;
}

static void cyc_complexity_finish_unit(void __unused *gcc_data, void __unused *user_data)
{
	fclose(log_file);
}

#define PASS_NAME cyc_complexity

#define NO_GATE
#define TODO_FLAGS_FINISH TODO_dump_func

#include "gcc-generate-gimple-pass.h"

__visible int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version)
{
	const char * const plugin_name = plugin_info->base_name;
	const char * filename = "cyc_complexity.out";

	PASS_INFO(cyc_complexity, "ssa", 1, PASS_POS_INSERT_AFTER);

	if (!plugin_default_version_check(version, &gcc_version)) {
		error(G_("incompatible gcc/plugin versions"));
		return 1;
	}

	// TODO: Find when to close the file so we don't leak
	/* register_callback (plugin_name, PLUGIN_FINISH_UNIT, */
	/* 			&cyc_complexity_finish_unit, NULL); */
	register_callback(plugin_name, PLUGIN_INFO, NULL,
				&cyc_complexity_plugin_info);
	register_callback(plugin_name, PLUGIN_PASS_MANAGER_SETUP, NULL,
				&cyc_complexity_pass_info);

	log_file = fopen(filename, "w");
	if (!log_file)
		error("Can't open %s for writing.", filename);

	return 0;
}
