
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
void cmpl_do(const char *dir)
{
	pid_t new_cmpl_pid = fork();
	int wait_stat;
	if (new_cmpl_pid > 0) {
		if (waitpid(new_cmpl_pid, &wait_stat, 0) > 0) {
			if (WIFEXITED(wait_stat)) {
				switch (WEXITSTATUS(wait_stat)) {
					/* TODO */
				}
				/* else branch normally should not happen */
			} else {
				if (WIFSIGNALED(wait_stat)) {
					cmpl_error(MT_CHILD_SIGNALED,
					    PINFO_ERROR,
					    TRUE,
					    "compiler child signaled with code %d",
					    WTERMSIG(wait_stat));
				}
			}
		} else {
			cmpl_error(MT_INTERN, PINFO_ERROR, TRUE, "waitpid");
		}
	}
}
