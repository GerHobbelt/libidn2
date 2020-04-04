// Just a test for static linking.
// We call one function from each object file in libwget_common
// Unresolved references should come up on linking.

#include <config.h>
#include <stringprep.h>

int main(void)
{
	stringprep_check_version (NULL);
	stringprep_utf8_to_ucs4 (NULL, -1, NULL);
}
