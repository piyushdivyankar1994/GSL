/* $Id$ */
/**
  zufall...
  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "zuf.h"

/* It is crucial that m == n-273 mod 607 at all times;
 * For speed of execution, however, this is never enforced.
 * Instead is is set in the initializer: note 607-273=334
 * Note also that the state.u[607] is not initialized
 */
typedef struct {
    int n,m;
    double u[607];
} gsl_ran_zuf_randomState;


inline double gsl_ran_zuf_uniform_wstate(void *vState)
{
    double t;
    gsl_ran_zuf_randomState *theState;
    theState = (gsl_ran_zuf_randomState *)vState;

    t = theState->u[theState->m] + theState->u[theState->n];
    while (t>1.0) t -= 1.0;     /* same as floor() */
    theState->u[theState->n] = t;
    if (++theState->n >= 607) theState->n=0;
    if (++theState->m >= 607) theState->m=0;
    
    return t;
}    

#define gsl_ran_zuf_RANDMAX 2147483647.0
inline unsigned long gsl_ran_zuf_random_wstate(void *vState)
{
    return (unsigned long)(gsl_ran_zuf_uniform_wstate(vState)*gsl_ran_zuf_RANDMAX);
}
double gsl_ran_zuf_max()
{
    return gsl_ran_zuf_RANDMAX;
}
void gsl_ran_zuf_seed_wstate(void *vState, int seed)
{
    /* A very elaborate seeding procedure is provided with the
     * zufall package; this is virtually a copy of that procedure */
    
    /* Initialized data */

    int kl = 9373;
    int ij = 1802;

    /* Local variables */
    int i, j, k, l, m;
    double s, t;
    int ii, jj;

    gsl_ran_zuf_randomState *theState;
    theState = (gsl_ran_zuf_randomState *)vState;

    /* It is crucial that m == n-273 mod 607 at all times;
     * For speed of execution, however, this is never enforced.
     * Instead is is set here (and in the state initializer):
     * note 607-273=334
     */
    theState->n=0;
    theState->m=334;
    
/*  generates initial seed buffer by linear congruential */
/*  method. Taken from Marsaglia, FSU report FSU-SCRI-87-50 */
/*  variable seed should be 0 < seed <31328 */


    if (seed != 0) {
	ij = seed;
    }

    i = ij / 177 % 177 + 2;
    j = ij % 177 + 2;
    k = kl / 169 % 178 + 1;
    l = kl % 169;
    for (ii = 0; ii < 607; ++ii) {
	s = 0.;
	t = .5;
	for (jj = 1; jj <= 24; ++jj) {
	    m = i * j % 179 * k % 179;
	    i = j;
	    j = k;
	    k = m;
	    l = (l * 53 + 1) % 169;
	    if (l * m % 64 >= 32) {
		s += t;
	    }
	    t *= 0.5;
	}
	theState->u[ii] = s;
    }
} 

/* It is crucial that m == n-273 mod 607 at all times;
 * For speed of execution, however, this is never enforced.
 * Instead is is set in the initializer: note 607-273=334.
 * Currently, state.u[607] is not initialized to anything special
 */
static gsl_ran_zuf_randomState state = {
    0, 334,
    { 0.1227935452,0.5911134421,0.3607630969,0.7119046445,0.3202328649,
      0.3050241354,0.6982676573,0.2516378998,0.4219606416,0.5938834879,
      0.6190659872,0.6368895552,0.3403881851,0.7912367710,0.7123150644,
      0.6839203127,0.7304019856,0.7056583688,0.7991272588,0.5948040367,
      0.9122974125,0.6838570197,0.6278226590,0.9260332470,0.8438239458,
      0.2833552260,0.3022383267,0.4406049712,0.0155517794,0.8440233620,
      0.5132938356,0.0151480259,0.2720487025,0.1861413345,0.8870551695,
      0.6152062181,0.6651946509,0.2172273432,0.5400686008,0.4357051088,
      0.4585446922,0.1523614912,0.8596130025,0.6935554703,0.5244628421,
      0.3507294205,0.5900121569,0.0522924955,0.0905593464,0.9870017008,
      0.3929665568,0.6722598616,0.0051221706,0.7262804611,0.2152673951,
      0.5612326402,0.8875126713,0.0329711230,0.1807737700,0.1818141944,
      0.5883307916,0.9888466722,0.0518799867,0.7971253749,0.4578459831,
      0.7236968549,0.4780066342,0.7745111105,0.2891836278,0.2758239091,
      0.8022396057,0.5962890915,0.2290897390,0.6243339456,0.4221766302,
      0.1454000717,0.7289557156,0.6831953840,0.8792172572,0.3277998457,
      0.5695290461,0.0261196613,0.1491773336,0.0142323324,0.0592143126,
      0.4871678918,0.6133816012,0.9243084465,0.5819582934,0.6934791205,
      0.4477096863,0.5748842251,0.5885751587,0.6091579979,0.0202396193,
      0.2064722821,0.5368177104,0.5020946837,0.6992671292,0.1486038205,
      0.1159293915,0.5653887787,0.9893227487,0.4985916549,0.5405234343,
      0.8103366576,0.7016353742,0.3233136039,0.8537737150,0.0537422881,
      0.0754986228,0.5943958098,0.9045804359,0.4452627404,0.1893607618,
      0.2101243348,0.1906052835,0.6570110598,0.9341863785,0.8925613295,
      0.0373141270,0.4673599517,0.6476560540,0.4704133191,0.6494350231,
      0.0094714139,0.9360559524,0.4530172858,0.9816194721,0.6695257681,
      0.1635516966,0.1914173409,0.7672311466,0.1015607996,0.5814983009,
      0.5566479012,0.4221282815,0.9386638538,0.2986042390,0.6358254843,
      0.3720969274,0.5047570176,0.0858485631,0.6250608319,0.1395680448,
      0.4477250976,0.7765290127,0.4972100353,0.1883622189,0.8659391119,
      0.1612819165,0.5332151595,0.6202026014,0.0362201305,0.0788267388,
      0.7058489551,0.8406456697,0.4690730038,0.7501167373,0.4121789681,
      0.8116622185,0.8527650626,0.8147886065,0.2150855924,0.9736910253,
      0.6171842397,0.3237004797,0.0970014429,0.6911926893,0.9304441346,
      0.7597026529,0.5913524555,0.8622272261,0.2552900799,0.7193628906,
      0.8903302864,0.6829005773,0.5042689000,0.0778983016,0.4469545151,
      0.9754088162,0.4979740223,0.4158795900,0.4739592399,0.1691432851,
      0.1638206704,0.8327762582,0.7065140023,0.3227300709,0.5743467028,
      0.3149165895,0.6412669329,0.4139184835,0.3071628786,0.8835354368,
      0.0331662472,0.5734030059,0.6752303808,0.9228139978,0.6674943387,
      0.8110163212,0.2740063979,0.9719277781,0.4811598491,0.7484071744,
      0.4932370437,0.5189635998,0.6660256092,0.8560858660,0.4924501970,
      0.6564168816,0.1203519639,0.0720346195,0.1536635098,0.8487083667,
      0.5601638593,0.7737528644,0.3517675679,0.6543871795,0.3472185191,
      0.7009906308,0.3133159243,0.0292077032,0.2057010650,0.9639690334,
      0.7452138753,0.5720219561,0.0506716769,0.2209360509,0.5285719656,
      0.6623706408,0.1206861061,0.8153758275,0.5664851346,0.1165158786,
      0.5948813013,0.6305072789,0.2959071079,0.5345575048,0.3602773480,
      0.8894188844,0.9775032238,0.5107110809,0.2565115376,0.7859394385,
      0.6734628570,0.1328949742,0.5046150179,0.8823642339,0.8233387032,
      0.2204196984,0.9680169946,0.7488192255,0.1542347930,0.9462622776,
      0.7086690029,0.4773549682,0.1604661997,0.1946051880,0.2372728647,
      0.9663067798,0.2981336887,0.0934234343,0.6192345268,0.1289963690,
      0.7226079039,0.3016665096,0.6365607097,0.9561481438,0.1203157257,
      0.5655995484,0.8891612969,0.3825075503,0.9991998682,0.6690479470,
      0.8828374573,0.6499913833,0.0184105928,0.9095403235,0.2323924380,
      0.2903029136,0.5565114126,0.7339196403,0.7369592464,0.5536312915,
      0.3965334953,0.4744712424,0.0115160344,0.7225949271,0.4466166054,
      0.4463798678,0.4147698381,0.9484232794,0.2626184423,0.1788705552,
      0.2590189727,0.1201510432,0.8903183325,0.6305383763,0.0601276206,
      0.3256069126,0.7484592760,0.7281738194,0.6516870116,0.3478359512,
      0.2702367771,0.2085480306,0.1845928503,0.4392217966,0.0906863562,
      0.7891529121,0.5023551807,0.3330217334,0.6054984431,0.4442243106,
      0.6899999219,0.3026385086,0.5271891090,0.6107203355,0.1810485329,
      0.9840185591,0.8002864856,0.2322324812,0.9651447031,0.4301468327,
      0.8306975087,0.2265942113,0.9339573181,0.1900441595,0.1958271882,
      0.4626697833,0.4100033739,0.2292521978,0.3894830476,0.4378165938,
      0.0688084071,0.5571733457,0.0125265769,0.3845688384,0.3907720530,
      0.3184157205,0.4405767233,0.8317128892,0.9078599824,0.8843304044,
      0.2620785106,0.7015387684,0.2502207917,0.2562158052,0.2942810745,
      0.8757030349,0.6427385849,0.2162948870,0.3686269796,0.8300367256,
      0.4746605151,0.6955403441,0.8296831893,0.9281675434,0.8702377414,
      0.7673051003,0.7887499377,0.2388931438,0.7379765997,0.2951026885,
      0.2051266306,0.9062019286,0.1767115851,0.8674757283,0.7671488477,
      0.0957903301,0.9700810106,0.9081869246,0.8431291985,0.7612066800,
      0.7670042948,0.1098093521,0.8708561840,0.4377631084,0.7458182287,
      0.1715052130,0.0052715202,0.4558842357,0.0251825359,0.3240553171,
      0.2961500902,0.2853356695,0.5784960114,0.7352607134,0.5531543035,
      0.9416006368,0.7765545852,0.5504575681,0.7528904446,0.7250590604,
      0.5813874693,0.0140229298,0.4415870160,0.7461207700,0.4193165123,
      0.0189259632,0.8006411048,0.8272271643,0.2656209478,0.0050183579,
      0.6141880473,0.6692112177,0.0405891936,0.8486469267,0.8059351493,
      0.3276898437,0.9491111708,0.6378902551,0.0524476944,0.6095188209,
      0.4790099619,0.2702172089,0.2368887321,0.1263787267,0.3530899507,
      0.6703169260,0.7662584165,0.4586578449,0.6401425775,0.6796211463,
      0.7731975331,0.1840115418,0.2541240216,0.3089506503,0.7234141326,
      0.7860007705,0.4631494710,0.2609288529,0.2753410700,0.6842509629,
      0.0344539259,0.9892334305,0.5397679524,0.4541641115,0.3067796596,
      0.2680721891,0.8351406364,0.6089055552,0.7307603240,0.3520700589,
      0.5989637140,0.4472144251,0.0952237092,0.8850272121,0.9588224250,
      0.7025943880,0.0440241829,0.4720700113,0.0604344821,0.9798277085,
      0.3203655232,0.9768136651,0.7841417442,0.2949502140,0.1211227847,
      0.2002280306,0.0057039119,0.2696216581,0.8041713314,0.8608592145,
      0.7644802995,0.7870098357,0.0792978590,0.3492631547,0.7893422113,
      0.6981977636,0.6241958206,0.3596673640,0.7465306318,0.0321970889,
      0.6832494901,0.0677468975,0.0230904282,0.7765897305,0.6792451660,
      0.2558299818,0.1979916333,0.9716267180,0.0137546624,0.1348547782,
      0.7017375082,0.9489140722,0.5283581333,0.8843343649,0.8485146854,
      0.6612537017,0.6492707727,0.9046392438,0.4097840241,0.1133730172,
      0.0375091943,0.0913322326,0.2627127469,0.9941388301,0.2221419013,
      0.8624324610,0.0626329000,0.0280541515,0.3978289510,0.1476849334,
      0.8752043235,0.9626380761,0.5619722477,0.9827842321,0.1648242723,
      0.9162901612,0.9213676962,0.5427636024,0.9392441104,0.8540478719,
      0.8344681086,0.3442425516,0.9714675897,0.7970152900,0.8915298744,
      0.6653832050,0.2649462603,0.5997369781,0.0770395868,0.8236373798,
      0.3624419533,0.0221903340,0.0044057053,0.5383704735,0.7222856823,
      0.5949457115,0.1942007239,0.7872581298,0.3734961788,0.4788751043,
      0.2678027621,0.4708928496,0.1172151358,0.3554391672,0.2653387382,
      0.5471547514,0.6768006254,0.4570486539,0.3218765915,0.2143738870,
      0.4840378291,0.0277137787,0.6196691773,0.8252064553,0.7245842107,
      0.6437277805,0.0498783784,0.2923058551,0.4769689799,0.7530738697,
      0.0119525983,0.7291006679,0.0475607545,0.0204105387,0.9834555276,
      0.7645471818,0.4313785795,0.9621648637,0.2497949391,0.9392633312,
      0.4844309385,0.3047329169,0.7488133362,0.3741997534,0.0946980552,
      0.0225400778,0.0688980923,0.8625267951,0.6607507830,0.7178479752,
      0.0377024375,0.0202546532,0.5883687532,0.8468680626,0.9854091275,
      0.1927149720,0.9535532750,0.6648633508,0.2654912604,0.0429180074,
      0.6987948194,0.8134976472,0.4101267133,0.8427150440,0.3528128420,
      0.4006965314,0.3428995870,0.3647316699,0.7479962525,0.9737294009,
      0.8804266567,0.1671841806,0.6062297516,0.1089957501,0.5169696209,
      0.7761999341,0.9221689852,0.8054923136,0.8831551215,0.2056173512,
      0.3734000081,0.7502747460 }
};

#include "zuf-state.c"

/* thats all... */
