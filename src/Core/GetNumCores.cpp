#ifdef _WIN32
	#include <windows.h>
#endif

int GetNumCores()
{
	int numCPU=0;

	#ifdef _WIN32
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		numCPU = sysinfo.dwNumberOfProcessors;
	#endif 
	#ifdef _LINUX
		numCPU = sysconf(_SC_NPROCESSORS_ONLN);
	#endif 
	#ifdef _FreeBSD
		int mib[4];
		size_t len; 

		/* set the mib for hw.ncpu */
		mib[0] = CTL_HW;
		mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

		/* get the number of CPUs from the system */
		sysctl(mib, 2, &numCPU, &len, NULL, 0);

		if(numCPU < 1) 
		{
     		mib[1] = HW_NCPU;
     		sysctl(mib, 2, &numCPU, &len, NULL, 0);

     		if(numCPU < 1)
     		{
          		numCPU = 1;
     		}
		}
	#endif
	#ifdef _HPUX
		numCPU = mpctl(MPC_GETNUMSPUS, NULL, NULL);
	#endif
	#ifdef _IRIX
		numCPU = sysconf(_SC_NPROC_ONLN);
	#endif
	return numCPU;
}
