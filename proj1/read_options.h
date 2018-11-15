/**
 * h File that reads options from user using getopt
 * @author SRI HARSHAL PARIMI
 * @author NIVEDITHA HARIHARAN
 */
int isOption(char *argv[], int optind);
int isValidArgument(char *optarg);
optionNode* setFlags(int argc, char* argv[]);
void printErrorMessage();
int parseOptions(int argc, char* argv[], char chopt);
int parsePidOption(int argc, char* argv[]);
