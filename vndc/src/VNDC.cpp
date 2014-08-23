#include "Zero.hpp"

#include "Funcs.hpp"

#include <unistd.h>
#include <signal.h>



void DebugTrap(int sig) {
	GetData()->debug_to_shell = true;
}

int main(int argc, char** argv) {
	printf(" ___________________________________________________________ \n");
	printf("| VNDC - VNDC is Not a Direct Clone (of VNDS ;P)            |\n");
	printf("| Interprets VNDS scripts with a few goodies and extensions |\n");
	printf("| (C) Jonathan Feldman 2014 - Under the MIT license         |\n");
	printf("| git: %s             |\n", GIT_REV);
	printf("|___________________________________________________________|\n\n");

	char* chdir_to_dir = NULL;
	char* main_script_override = NULL;
	char* save_file = NULL;
	bool vndc_extensions = true;
	int width = 0, height = 0;
	bool debug_enable = false;
	bool enable_v = false;
	char c;

	while((c = getopt(argc, argv, "bvx:y:d:m:s:ch")) != -1) {
		switch(c) {
			case 'v':
				printf("[info] Script commands will be echoed.\n");
				enable_v = true;
				break;
			case 'b':
				printf("[debug] Debug Mode enabled.\n");
				debug_enable = true;
				break;
			case 'd':
				printf("[info] Directory specified: %s\n", optarg);
				chdir_to_dir = optarg;
				break;
			case 'x':
				sscanf(optarg, "%d", &width);
				break;
			case 'y':
				sscanf(optarg, "%d", &height);
				chdir_to_dir = optarg;
				break;
			case 'm':
				printf("[debug] Main script overriden as: '%s'\n", optarg);
				main_script_override = optarg;
				break;
			case 's':
				printf("[info] Load save: '%s'\n", optarg);
				save_file = optarg;
				break;
			case 'c':
				printf("[debug] Compliant mode: all VNDC extensions off\n");
				vndc_extensions = false;
				break;
			case 'h':
				printf("-x size -y size\t\tSet display window to wxh\n");
				printf("-d dir\t\tChange to directory/Run game in directory\n");
				printf("-b\t\tDebug Mode. Hit Ctrl+C on console for shell\n");
				printf("-v\t\tVerbose. Echo script commands back as they execute\n");
				printf("-m .scr\t\tLoad .scr as main script\n");
				printf("-s .scr\t\tLoad save .scr\n");
				printf("-c\t\tCompliant mode; don't use VNDC extensions\n");
				printf("-h\t\tPrint this help message\n");
				return 0;
				break;
			default:
				break;
		}
	}

	CreateDataContainer();

	GetData()->ctx = new ContextManager();

	if(chdir_to_dir)
		chdir(chdir_to_dir);
	if(main_script_override != NULL) {
		printf("[debug] Note that in this mode, l_flags is not set.\n[debug] Expect bugs/gameover, this is normal.\n");
		memset(GetData()->main_scr[0], 0, 399);
		strncpy(GetData()->main_scr[0], main_script_override, 399);
	}
	GetData()->vndc_enabled = vndc_extensions;
	GetData()->verbose = enable_v;
	if(debug_enable) {
		signal(SIGINT, DebugTrap);
		GetData()->debug_mode = true;
	}

	if(width > 0 && height > 0) {
		GetData()->physical_w = width;
		GetData()->physical_h = height;
	}

	Setup();

	if(!save_file)
		op_jump(GetData()->main_scr[0], NULL, true);
	else
		op_jump(save_file, NULL, true);

	while(!(GetData()->ctx->GetQuit())) {
		Loop();
	}

	delete GetData()->ctx;
	delete GetData();
}
