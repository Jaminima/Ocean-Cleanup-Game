//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "Window.h"

#include <vector>

#include "Project.h"
#include "Asset.h"

int runTests(std::string value)
{
	doctest::Context ctx;
	if (value.compare("adv") == 0)
	{
		ctx.addFilter("test-case-exclude", "STD");
	}
	else
	{
		ctx.addFilter("test-case-exclude", "ADV");
	}
	ctx.setOption("abort-after", 10);  // default - stop after 5 failed asserts
	ctx.setOption("no-breaks", true); // override - don't break in the debugger
	int res = ctx.run();              // run test cases unless with --no-run
	if (ctx.shouldExit())              // query flags (and --exit) rely on this
		return res;                   // propagate the result of the tests
	// your actual program execution goes here - only if we haven't exited
	return res; // + your_program_res
}
//----------------------------------------------------------------------------
//
// main
//
int
main(int argc, char** argv)
{
	Asset a("Cube");

	a.LoadAsset();

	initWindow();
}