#include <iostream>
#include <fstream>
#include <cmath>

#include <boost/numeric/odeint.hpp>

#include "state_type.h"
#include "sys_SM.h"
#include "sys_SMEFT.h"
#include "sys_LEFT.h"
#include "read_input.h"
#include "param.h"

using namespace std;
using namespace boost::numeric::odeint;

const int STEP = 100;

int main(int argc, char **argv) {

string input_file1 = "input_SM.txt";
string input_file2 = "input_SMEFT.txt";

fstream ofile;
ofile.open("output1.txt", ios_base::out);

double log_hscale, log_ewscale, log_lscale;
double dth, dtl;

//EW parameters
double clq1_1_ew, clq1_2_ew, clq1_3_ew;
double clq3_1_ew, clq3_2_ew, clq3_3_ew;
double cqe_1_ew, cqe_2_ew, cqe_3_ew;

double ledvll_1_ew, ledvll_2_ew, ledvll_3_ew;
double ldevlr_1_ew, ldevlr_2_ew, ldevlr_3_ew;

//system functions
state_type g_run, gp_run, gs_run;
state_type gqcd_run, eqed_run;
state_type Gu33_run;

state_type clq1_1_run, clq1_2_run, clq1_3_run;
state_type clq3_1_run, clq3_2_run, clq3_3_run;
state_type cqe_1_run, cqe_2_run, cqe_3_run;

state_type ledvll_1_run, ledvll_2_run, ledvll_3_run;
state_type ldevlr_1_run, ldevlr_2_run, ldevlr_3_run;

//read input from files
read_sm_input(input_file1);
read_smeft_input(input_file2);

log_hscale = log(hscale);
log_ewscale = log(ewscale);
log_lscale = log(lscale);

dth = (log_hscale-log_ewscale)/STEP;
dtl = (log_lscale-log_ewscale)/STEP;
cout<<dth<<endl;

g_run[0] = g_ew;
gp_run[0] = gp_ew;
gs_run[0] = gs_ew;
Gu33_run[0] = Gu33_ew;

for(size_t i=0; i<STEP; ++i) {
  sys_Gu33 sf_Gu33(g_run[0], gp_run[0], gs_run[0]);
  runge_kutta4< state_type > rk;
  rk.do_step(sf_Gu33, Gu33_run, log_ewscale, dth);
  rk.do_step(sys_g, g_run, log_ewscale, dth);
  rk.do_step(sys_gp, gp_run, log_ewscale, dth);
  rk.do_step(sys_gs, gs_run, log_ewscale, dth);
  log_ewscale = log_ewscale + dth;
} 

clq1_1_run[0] = clq1_1_h;
clq1_2_run[0] = clq1_2_h;
clq1_3_run[0] = clq1_3_h;

clq3_1_run[0] = clq3_1_h;
clq3_2_run[0] = clq3_2_h;
clq3_3_run[0] = clq3_3_h;

cqe_1_run[0] = cqe_1_h;
cqe_2_run[0] = cqe_2_h;
cqe_3_run[0] = cqe_3_h;

cout<<"high scale: "<<exp(log_ewscale)<<endl;
cout<<"g="<<g_run[0]<<endl;
cout<<"gp="<<gp_run[0]<<endl;
cout<<"gs="<<gs_run[0]<<endl;
cout<<"Gu33="<<Gu33_run[0]<<endl;
cout<<"clq1_1="<<clq1_1_run[0]<<endl;
cout<<"clq1_2="<<clq1_2_run[0]<<endl;
cout<<"clq1_3="<<clq1_3_run[0]<<endl;
cout<<"clq3_1="<<clq3_1_run[0]<<endl;
cout<<"clq3_2="<<clq3_2_run[0]<<endl;
cout<<"clq3_3="<<clq3_3_run[0]<<endl;
cout<<"cqe_1="<<cqe_1_run[0]<<endl;
cout<<"cqe_2="<<cqe_2_run[0]<<endl;
cout<<"cqe_3="<<cqe_3_run[0]<<endl;

for(size_t i=0; i<STEP; ++i) {
  sys_lq1 sf_lq1_1(g_run[0], gp_run[0], Gu33_run[0], clq1_2_run[0], clq1_3_run[0], clq3_1_run[0], cqe_1_run[0], cqe_2_run[0], cqe_3_run[0]);
  sys_lq1 sf_lq1_2(g_run[0], gp_run[0], Gu33_run[0], clq1_3_run[0], clq1_1_run[0], clq3_2_run[0], cqe_1_run[0], cqe_2_run[0], cqe_3_run[0]);
  sys_lq1 sf_lq1_3(g_run[0], gp_run[0], Gu33_run[0], clq1_1_run[0], clq1_2_run[0], clq3_3_run[0], cqe_1_run[0], cqe_2_run[0], cqe_3_run[0]);
  sys_lq3 sf_lq3_1(g_run[0], gp_run[0], Gu33_run[0], clq3_2_run[0], clq3_3_run[0], clq1_1_run[0]);
  sys_lq3 sf_lq3_2(g_run[0], gp_run[0], Gu33_run[0], clq3_3_run[0], clq3_1_run[0], clq1_2_run[0]);
  sys_lq3 sf_lq3_3(g_run[0], gp_run[0], Gu33_run[0], clq3_1_run[0], clq3_2_run[0], clq1_3_run[0]);
  sys_qe sf_qe_1(gp_run[0], Gu33_run[0], cqe_2_run[0], cqe_3_run[0], clq1_1_run[0], clq1_2_run[0], clq1_3_run[0]);
  sys_qe sf_qe_2(gp_run[0], Gu33_run[0], cqe_3_run[0], cqe_1_run[0], clq1_1_run[0], clq1_2_run[0], clq1_3_run[0]);
  sys_qe sf_qe_3(gp_run[0], Gu33_run[0], cqe_1_run[0], cqe_2_run[0], clq1_1_run[0], clq1_2_run[0], clq1_3_run[0]);
  sys_Gu33 sf_Gu33(g_run[0], gp_run[0], gs_run[0]);
  runge_kutta4< state_type > rk;
  rk.do_step(sf_lq1_1, clq1_1_run, log_ewscale, -dth);
  rk.do_step(sf_lq1_2, clq1_2_run, log_ewscale, -dth);
  rk.do_step(sf_lq1_3, clq1_3_run, log_ewscale, -dth);
  rk.do_step(sf_lq3_1, clq3_1_run, log_ewscale, -dth);
  rk.do_step(sf_lq3_2, clq3_2_run, log_ewscale, -dth);
  rk.do_step(sf_lq3_3, clq3_3_run, log_ewscale, -dth);
  rk.do_step(sf_qe_1, cqe_1_run, log_ewscale, -dth);
  rk.do_step(sf_qe_2, cqe_2_run, log_ewscale, -dth);
  rk.do_step(sf_qe_3, cqe_3_run, log_ewscale, -dth);
  rk.do_step(sf_Gu33, Gu33_run, log_ewscale, -dth);
  rk.do_step(sys_g, g_run, log_ewscale, -dth);
  rk.do_step(sys_gp, gp_run, log_ewscale, -dth);
  rk.do_step(sys_gs, gs_run, log_ewscale, -dth);
  log_ewscale = log_ewscale - dth;
  //ofile<<exp(log_ewscale)<<" "<<cqe_1_run[0]<<endl; 
}

cout<<"EW scale: "<<exp(log_ewscale)<<endl;
cout<<"g="<<g_run[0]<<endl;
cout<<"gp="<<gp_run[0]<<endl;
cout<<"gs="<<gs_run[0]<<endl;
cout<<"Gu33="<<Gu33_run[0]<<endl;
cout<<"clq1_1="<<clq1_1_run[0]<<endl;
cout<<"clq1_2="<<clq1_2_run[0]<<endl;
cout<<"clq1_3="<<clq1_3_run[0]<<endl;
cout<<"clq3_1="<<clq3_1_run[0]<<endl;
cout<<"clq3_2="<<clq3_2_run[0]<<endl;
cout<<"clq3_3="<<clq3_3_run[0]<<endl;
cout<<"cqe_1="<<cqe_1_run[0]<<endl;
cout<<"cqe_2="<<cqe_2_run[0]<<endl;
cout<<"cqe_3="<<cqe_3_run[0]<<endl;

clq1_1_ew = clq1_1_run[0];
clq1_2_ew = clq1_2_run[0];
clq1_3_ew = clq1_3_run[0];

clq3_1_ew = clq3_1_run[0];
clq3_2_ew = clq3_2_run[0];
clq3_3_ew = clq3_3_run[0];

cqe_1_ew = cqe_1_run[0];
cqe_2_ew = cqe_2_run[0];
cqe_3_ew = cqe_3_run[0];

//matching
ledvll_1_ew = clq1_1_ew+clq3_1_ew;
ledvll_2_ew = clq1_2_ew+clq3_2_ew;
ledvll_3_ew = clq1_3_ew+clq3_3_ew;

ldevlr_1_ew = cqe_1_ew;
ldevlr_2_ew = cqe_2_ew;
ldevlr_3_ew = cqe_3_ew;

eqed_run[0] = eqed_ew;
gqcd_run[0] = gqcd_ew;

ledvll_1_run[0] = ledvll_1_ew;
ledvll_2_run[0] = ledvll_2_ew;
ledvll_3_run[0] = ledvll_3_ew;

ldevlr_1_run[0] = ldevlr_1_ew;
ldevlr_2_run[0] = ldevlr_2_ew;
ldevlr_3_run[0] = ldevlr_3_ew;

cout<<"EW scale: "<<exp(log_ewscale)<<endl;
cout<<"eqed="<<eqed_run[0]<<endl;
cout<<"gqcd="<<gqcd_run[0]<<endl;
cout<<"ledvll_1="<<ledvll_1_run[0]<<endl;
cout<<"ledvll_2="<<ledvll_2_run[0]<<endl;
cout<<"ledvll_3="<<ledvll_3_run[0]<<endl;
cout<<"ldevlr_1="<<ldevlr_1_run[0]<<endl;
cout<<"ldevlr_2="<<ldevlr_2_run[0]<<endl;
cout<<"ldevlr_3="<<ldevlr_3_run[0]<<endl;

for(size_t i=0; i<STEP; ++i) {
  sys_edvll sf_edvll_1(eqed_run[0], ledvll_2_run[0], ledvll_3_run[0], ldevlr_1_run[0], ldevlr_2_run[0], ldevlr_3_run[0]);
  sys_edvll sf_edvll_2(eqed_run[0], ledvll_3_run[0], ledvll_1_run[0], ldevlr_1_run[0], ldevlr_2_run[0], ldevlr_3_run[0]);
  sys_edvll sf_edvll_3(eqed_run[0], ledvll_1_run[0], ledvll_2_run[0], ldevlr_1_run[0], ldevlr_2_run[0], ldevlr_3_run[0]);
  sys_devlr sf_devlr_1(eqed_run[0], ldevlr_2_run[0], ldevlr_3_run[0], ledvll_1_run[0], ledvll_2_run[0], ledvll_3_run[0]);
  sys_devlr sf_devlr_2(eqed_run[0], ldevlr_3_run[0], ldevlr_1_run[0], ledvll_1_run[0], ledvll_2_run[0], ledvll_3_run[0]);
  sys_devlr sf_devlr_3(eqed_run[0], ldevlr_1_run[0], ldevlr_2_run[0], ledvll_1_run[0], ledvll_2_run[0], ledvll_3_run[0]);
  runge_kutta4< state_type > rk;
  rk.do_step(sf_edvll_1, ledvll_1_run, log_ewscale, dtl);
  rk.do_step(sf_edvll_2, ledvll_2_run, log_ewscale, dtl);
  rk.do_step(sf_edvll_3, ledvll_3_run, log_ewscale, dtl);
  rk.do_step(sf_devlr_1, ldevlr_1_run, log_ewscale, dtl);
  rk.do_step(sf_devlr_2, ldevlr_2_run, log_ewscale, dtl);
  rk.do_step(sf_devlr_3, ldevlr_3_run, log_ewscale, dtl);
  rk.do_step(sys_eqed, eqed_run, log_ewscale, dtl);
  rk.do_step(sys_gqcd, gqcd_run, log_ewscale, dtl);
  log_ewscale = log_ewscale + dtl;
  //ofile<<exp(log_ewscale)<<" "<<ldevlr_1_run[0]<<endl; 
  ofile<<exp(log_ewscale)<<" "<<ldevlr_1_run[0]<<endl; 
}

cout<<"low scale: "<<exp(log_ewscale)<<endl;
cout<<"eqed="<<eqed_run[0]<<endl;
cout<<"gqcd="<<gqcd_run[0]<<endl;
cout<<"ledvll_1="<<ledvll_1_run[0]<<endl;
cout<<"ledvll_2="<<ledvll_2_run[0]<<endl;
cout<<"ledvll_3="<<ledvll_3_run[0]<<endl;
cout<<"ldevlr_1="<<ldevlr_1_run[0]<<endl;
cout<<"ldevlr_2="<<ldevlr_2_run[0]<<endl;
cout<<"ldevlr_3="<<ldevlr_3_run[0]<<endl;

return 0;
}//end of main
