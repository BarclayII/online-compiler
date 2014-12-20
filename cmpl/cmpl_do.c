#include "config.h"
#include "const.h"
#include "cmpl.h"

/*
 * Compiler process compiles in the following way:
 * 1. Spawn a child process and waits for it to complete.
 * 2. For child process, change into the specified directory and finds for
 *    Makefile.
 * 3. If Makefile exists in the directory, it directly calls GNU (or other)
 *    make utility.  Otherwise:
 * 4. Parses every lex and yacc files to C source files and headers with
 *    default settings, compiles them to .o files and links them together,
 *    again with default settings.
 */

void cmpl_do_makefile()
{
	const char *mkprog = MAKE;
	pid_t pid;
	int wait_stat;

	int out_fd, err_fd;

	if ((pid = fork()) > 0) {
		if (waitpid(pid, &wait_stat, 0) == -1) {
			/* do not check for error on kill() since an error
			 * will be reported anyway. */
			cmpl_error(MT_INTERN, PINFO_ERROR, TRUE, "waitpid()");
			kill(pid, SIGTERM);
			exit(CMPL_EXIT_FAIL);
		} else if (WIFSIGNALED(wait_stat)) {
			cmpl_error(MT_CHILD_SIGNALED, PINFO_ERROR, FALSE,
			    "cmpl_do_makefile() child signaled %d",
			    WTERMSIG(wait_stat));
			exit(CMPL_EXIT_FAIL);
		} else if (WIFEXITED(wait_stat)) {
			switch (WEXITSTATUS(wait_stat)) {
				/* TODO */
			}
		} else {
			/* NOTREACHED */
			cmpl_error(MT_INTERN, PINFO_ERROR, FALSE,
			    "child neither exited nor signaled");
			exit(CMPL_EXIT_FAIL);
		}
	} else {
		/* redirect standard output and standard error */
		if (redirect(NULL, MAKE_OUT, MAKE_ERR) != 0) {
			cmpl_error(MT_INTERN, PINFO_ERROR, FALSE,
			    "redirect make output and error failed");
			exit(CMPL_EXIT_FAIL);
		}

		/* invoke make utility, never returns */
		if (execve(mkprog, NULL, NULL) == -1) {
			cmpl_error(MT_INTERN, PINFO_ERROR, TRUE,
			    "execve %s", mkprog);
			exit(CMPL_EXIT_FAIL);
		}

		/* NOTREACHED */
		cmpl_error(MT_INTERN, PINFO_ERROR, FALSE, "passed execve()?");
		exit(CMPL_EXIT_FAIL);
	}
}

/*
 * Recursively traverse current directory and process each entry accordingly
 */
void cmpl_traverse(void)
{
	struct dirent **de;

	int entries = scandir(dir, &de, NULL, alphasort);
	int i;
	
	for (i = 0; i < entries; ++i) {
		switch (de[i]->d_type) {
		case DT_DIR:
			chdir(de[i]->d_name);
			cmpl_traverse();
			chdir("..");
			break;
		case DT_REG:
			cmpl_process_file(de[i]->d_name);
			break;
		}
		free_n(&(de[i]->d_name));
		free_n(&(de[i]));
	}
	free_n(&de);
}

void cmpl_do_child_default(void)
{
	/* traverse the directory and process each file */
	cmpl_traverse();
	/* TODO: link every object into a single executable a.out */
}

void cmpl_do_child(const char *dir)
{
	if (chdir(dir) == -1) {
		pinfo(PINFO_ERROR, TRUE, "chdir()");
		exit(CMPL_EXIT_FAIL);
	}

	int mk_fd = open("Makefile", O_RDONLY);
	if (mk_fd == -1) {
		/* Makefile does not exist, change to default behavior */
		cmpl_do_child_default();
	} else {
		close(mk_fd);
		cmpl_do_child_makefile();
	}

	exit(CMPL_EXIT_SUCCESS);
}

void cmpl_do(const char *dir)
{
	pid_t new_cmpl_pid;
	int wait_stat;
	if ((new_cmpl_pid = fork()) > 0) {
		if (waitpid(new_cmpl_pid, &wait_stat, 0) > 0) {
			if (WIFEXITED(wait_stat)) {
				switch (WEXITSTATUS(wait_stat)) {
					/* TODO */
				}
				/* else branch normally should not happen */
			} else if (WIFSIGNALED(wait_stat)) {
				cmpl_error(MT_CHILD_SIGNALED,
				    PINFO_ERROR,
				    TRUE,
				    "compiler child signaled %d",
				    WTERMSIG(wait_stat));
			} else {
				/* NOTREACHED */
				cmpl_error(MT_INTERN, PINFO_ERROR, FALSE,
				    "child neither exited nor signaled");
			}
		} else {
			cmpl_error(MT_INTERN, PINFO_ERROR, TRUE, "waitpid");
		}
	} else {
		cmpl_do_child(dir);
		/* NOTREACHED */
		cmpl_error(MT_INTERN, PINFO_ERROR, FALSE,
		    "return from cmpl_do_child()");
	}
}

