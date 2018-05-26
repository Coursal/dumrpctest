/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "dum.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
dum_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		X_arr average_1_arg;
		X_arr max_and_min_1_arg;
		X_times_r product_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case average:
		_xdr_argument = (xdrproc_t) xdr_X_arr;
		_xdr_result = (xdrproc_t) xdr_float;
		local = (char *(*)(char *, struct svc_req *)) average_1_svc;
		break;

	case max_and_min:
		_xdr_argument = (xdrproc_t) xdr_X_arr;
		_xdr_result = (xdrproc_t) xdr_max_min;
		local = (char *(*)(char *, struct svc_req *)) max_and_min_1_svc;
		break;

	case product:
		_xdr_argument = (xdrproc_t) xdr_X_times_r;
		_xdr_result = (xdrproc_t) xdr_prod;
		local = (char *(*)(char *, struct svc_req *)) product_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (DUM_PROG, DUM_VERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, DUM_PROG, DUM_VERS, dum_prog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (DUM_PROG, DUM_VERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, DUM_PROG, DUM_VERS, dum_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (DUM_PROG, DUM_VERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
