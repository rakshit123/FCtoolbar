#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
/*
  Use getopt_long_only()
*/
/* This structure represents the arguments */
struct arguments
{
  int flag_usage;                          /* The -usage flag */
  int flag_help;                           /* The -help flag */
  int correction_motion;                   /* The -m flag */
  int correction_exp_conditions;           /* The -e flag */
  int correction_gm;                       /* The -g flag */
  int correction_wm_ventricle;             /* The -v flag */
  int correction_caridac_respiratory;      /* The -r flag */    
  int correlation_method;                  /* Argument for Correlation Method */
  int num_pc;                              /* Number of principal components for PCA*/
  int roi;                                 /* Argument for Region of interest */
  int rep_ts;                              /* Argument for selecting represntative time series */
  char *dset;                              /* Dataset */
  float voxel_x, voxel_y, voxel_z;           /* x,y,z coordinates of single voxel */
  float cube_dim;                            /* side/2 of cube ROI */
  float cuboid_x, cuboid_y, cuboid_z;        /* length/2, breadth/2, height/2 of cuboid ROI */
  float ellipse_rx, ellipse_ry, ellipse_rz;  /* rx/2,ry/2,rz/2 for ellipse ROI */
  float sphere_r;                            /* Radius of sphere ROI */
  int atlas_reg;                        /* Atlas region number */

};

enum{
  OPT_ROI = 1000,
  OPT_REP_TS,
  OPT_CORR_METHOD,
  OPT_DSET,
  OPT_NUM_PC,
  OPT_VOXEL_X, OPT_VOXEL_Y, OPT_VOXEL_Z,
  OPT_CUBE_DIM,
  OPT_CUBOID_X, OPT_CUBOID_Y, OPT_CUBOID_Z,
  OPT_ELLIPSE_RX, OPT_ELLIPSE_RY, OPT_ELLIPSE_RZ,
  OPT_SPHERE_R,
  OPT_ATLAS_REG,
  OPT_CORR_MOTION,
  OPT_CORR_EXP_COND,
  OPT_CORR_GM,
  OPT_CORR_WM_VENTRICLE,
  OPT_CARDIAC_RESPRATORY,
  OPT_FLAG_USAGE,
  OPT_FLAG_HELP
  
};  

static struct option long_options [] = {
  {"roi", required_argument, NULL, OPT_ROI},
  {"rep_time_series", required_argument, NULL, OPT_REP_TS},
  {"corr_method", required_argument, NULL, OPT_CORR_METHOD},
  {"dset", required_argument, NULL, OPT_DSET},
  {"num_pc", required_argument, NULL, OPT_NUM_PC},
  {"voxel_x", required_argument, NULL, OPT_VOXEL_X},
  {"voxel_y", required_argument, NULL, OPT_VOXEL_Y},
  {"voxel_z", required_argument, NULL, OPT_VOXEL_Z},
  {"cube_len", required_argument, NULL, OPT_CUBE_DIM},
  {"cuboid_x", required_argument, NULL, OPT_CUBOID_X},
  {"cuboid_y", required_argument, NULL, OPT_CUBOID_Y},
  {"cuboid_z", required_argument, NULL, OPT_CUBOID_Z},
  {"ellipse_rx", required_argument, NULL, OPT_ELLIPSE_RX},
  {"ellipse_ry", required_argument, NULL, OPT_ELLIPSE_RY},
  {"ellipse_rz", required_argument, NULL, OPT_ELLIPSE_RZ},
  {"sphere_r", required_argument, NULL, OPT_SPHERE_R},
  {"atlas_reg", required_argument, NULL, OPT_ATLAS_REG},
  {"correction_motion", no_argument, NULL, OPT_CORR_MOTION},
  {"correction_exp_cond", no_argument, NULL, OPT_CORR_EXP_COND},
  {"correction_gm", no_argument, NULL, OPT_CORR_GM},
  {"correction_wm_ventricle", no_argument, NULL, OPT_CORR_WM_VENTRICLE},
  {"correction_cardiac_respiratory", no_argument, NULL, OPT_CARDIAC_RESPRATORY},
  {"usage", no_argument, NULL, OPT_FLAG_USAGE},
  {"help", no_argument, NULL, OPT_FLAG_HELP},
  
};



void print_help()
{
  printf("Help needed\n");
  printf("fc.c computes functional connectivity\nUse the following command line arguments:\n");
  printf("I) Select Region of interest by -roi=INT\n\t0 for Single Voxel\n\t1 for Cube\n\t2 for Cuboid\n\t3 for Sphere\n\t4 for Ellipse\n\t5 for Atlas Region\n\n");
  printf("II) Select Reprentative Time Series by -rep_ts=INT\n\t0 for Single Voxel\n\t1 for Average of all the voxels in ROI\n\t2 for PCA of ROI\n\t3 for Each voxel in ROI with every other voxel\n\n");
  printf("III) Select Co-relation method by -corr_method=INT\n\t0 for Pearson\n\t1 for Mutual Information\n\t2 for Binary\n\t3 for Kendall Coefficient of Concordance (KCC)\n\n");
  printf("IV) Artifact removal\nInclude the following falgs for removal of the following artifacts:\n\t-correction_motion for motion correction\n\t-correction_exp_cond for correction due to experimental conditions\n\t-correction_gm for removal of average Gray Matter Signal\n\t-correction_wm_ventricle for removal of artifacts by White matter and ventricle signal\n\t-correction_cardiac_respiratory for removal of artifacts by cardiac and respiratory signal\n\n");
  printf("V) ROI options:\n\ta) Single Voxel:\n\t\t-voxel_x=FLOAT for X coordinate in mm\n\t\t-voxel_y=FLOAT for Y coordinate in mm\n\t\t-voxel_z=FLOAT for Z coordinate in mm\n\tb) Cube:\n\t\t-cube_len=FLOAT for length of side of cube or half side of cube(To be decided)\n\tc) Cuboid:\n\t\t-cuboid_x=FLOAT for length of cuboid in mm\n\t\t-cuboid_y=FLOAT for breadth of cuboid in mm\n\t\t-cuboid_z=FLOAT for height of cuboid in mm\n\t\t\n\td) Sphere:\n\t\t-sphere_r=FLOAT for radius of sphere in mm\n\te) Ellipse:\n\t\t-ellipse_rx=FLOAT for RX in mm\n\t\t-ellipse_ry=FLOAT for RY in mm\n\t\t-ellipse_rz=FLOAT for RZ in mm\n\tf) Atlas Region:\n\t\t-atlas_reg=INT for selecting an atlas region\n");
  printf("VI) PCA options: \n\tSet number of principal components for PCA by -num_pc=INT\n\tINT should be between 1 and 5\n\tDefault is 1\n");
  printf("VII) Daaset options:\n\tChoose your own dataset by -dset=STRING\n\tDefault is \"_______\"\n");
  printf("VIII) -help to display this help\n\n");
  printf("IX)   -usage to display usage information\n\n");
}

void print_usage()
{
  printf("\nHow to use it?\n");
  print_help();
  
}


int long_index;
struct arguments argum;


int parse_long(int argc, char * argv[])
{
  int opt = 0;
  while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1)
  {
    switch(opt)
    {
      case OPT_ROI: 
        argum.roi = atoi(optarg);
        break;
        
      case OPT_REP_TS:
        argum.rep_ts = atoi(optarg);
        break;
      case OPT_CORR_METHOD:
        argum.correlation_method = atoi(optarg);
        break;
      case OPT_DSET:
        argum.dset = optarg; // can fail. copy string for correctness
        break;
      case OPT_NUM_PC:
        argum.num_pc = atoi(optarg);
        break;
      case OPT_VOXEL_X:
        argum.voxel_x = atof(optarg); // float for more accuracy and correctness
        break;
      case OPT_VOXEL_Y:
        argum.voxel_y = atof(optarg);
        break;
        
      case OPT_VOXEL_Z:
        argum.voxel_z = atof(optarg);
        break;
         
      case OPT_CUBE_DIM:
        argum.cube_dim = atof(optarg);
        break;
         
      case OPT_CUBOID_X:
        argum.cuboid_x = atof(optarg);
        break;
        
      case OPT_CUBOID_Y:
        argum.cuboid_y = atof(optarg);
        break;
        
      case OPT_CUBOID_Z:
        argum.cuboid_z = atof(optarg);
        break;
      case OPT_ELLIPSE_RX:
        argum.ellipse_rx = atof(optarg);
        break;
        
      case OPT_ELLIPSE_RY:
        argum.ellipse_ry = atof(optarg);
        break;
        
      case OPT_ELLIPSE_RZ:
        argum.ellipse_rz = atof(optarg);
        
      case OPT_SPHERE_R:
        argum.sphere_r = atof(optarg);
        break;
      case OPT_ATLAS_REG:
        argum.atlas_reg = atoi(optarg);
        break;
        
      case OPT_CORR_MOTION: // has no arguments!!
        argum.correction_motion = 1;
        break;
      case OPT_CORR_EXP_COND:
        argum.correction_exp_conditions = 1;
        break;
      case OPT_CORR_GM:
        argum.correction_gm = 1;
        break;
        
      case OPT_CORR_WM_VENTRICLE:
        argum.correction_wm_ventricle = 1;
        break;
      case OPT_CARDIAC_RESPRATORY:
        argum.correction_caridac_respiratory = 1;
      case OPT_FLAG_USAGE:
        argum.flag_usage = 1;
        print_usage();
        exit(EXIT_SUCCESS);
        break;
        
      case OPT_FLAG_HELP:
        argum.flag_help = 1;
        print_help();
        exit(EXIT_SUCCESS);
        break;  
      case '?':
        //printf("Either %s is not a vaild argument or it is missing its argument value\n",long_options[long_index].name);
        //printf("Either %s is not a vaild argument or it is missing its return value%d\n",optarg,long_index);
        exit(EXIT_SUCCESS);
        break;
      default:
         printf("No match for %d",opt);
         print_usage();
         exit(EXIT_SUCCESS);
         break;    
    }
  }
  return 0;
}

void print_args()
{
  printf("flag_usage - %d\n",argum.flag_usage);                          /* The -usage flag */
  printf("flag_help - %d\n",argum.flag_help);                            /* The -help flag */
  printf("correction_motion - %d\n",argum.correction_motion);                    /* The -m flag */
  printf("correction_exp_conditions - %d\n", argum.correction_exp_conditions);           /* The -e flag */
  printf("correction_gm - %d\n", argum.correction_gm);                       /* The -g flag */
  printf("correction_wm_ventricle - %d\n", argum.correction_wm_ventricle);             /* The -v flag */
  printf("correction_caridac_respiratory - %d\n", argum.correction_caridac_respiratory);      /* The -r flag */    
  printf("correlation_method - %d\n", argum.correlation_method);                  /* Argument for Correlation Method */
  printf("num_pc - %d\n", argum.num_pc);                              /* Number of principal components for PCA*/
  printf("ROI - %d\n", argum.roi);                                 /* Argument for Region of interest */
  printf("rep_ts - %d\n", argum.rep_ts);                              /* Argument for selecting represntative time series */
  printf("dataset - %s\n", argum.dset);                              /* Dataset */
  printf("voxel - x %f , y %f , z %f\n",  argum.voxel_x, argum.voxel_y, argum.voxel_z);           /* x,y,z coordinates of single voxel */
  printf("cube_dim - %f\n",  argum.cube_dim);                            /* side/2 of cube ROI */
  printf("cuboid dim - x %f , y %f , z %f\n", argum.cuboid_x, argum.cuboid_y, argum.cuboid_z);        /* length/2, breadth/2, height/2 of cuboid ROI */
  printf("ellpise dim - rx %f , ry %f , rz %f\n", argum.ellipse_rx, argum.ellipse_ry, argum.ellipse_rz);  /* rx/2,ry/2,rz/2 for ellipse ROI */
  printf("Sphere_r - %f\n", argum.sphere_r);                            /* Radius of sphere ROI */
  printf("Atlas region - %d\n", argum.atlas_reg);                        /* Atlas region number */

}

void init_args()
{
  argum.flag_usage = 0;                          /* The -usage flag */
  argum.flag_help = 0;                           /* The -help flag */
  argum.correction_motion = 0;                   /* The -m flag */
  argum.correction_exp_conditions = 0;           /* The -e flag */
  argum.correction_gm = 0;                       /* The -g flag */
  argum.correction_wm_ventricle = 0;             /* The -v flag */
  argum.correction_caridac_respiratory = 0;      /* The -r flag */    
  argum.correlation_method = 0;                  /* Argument for Correlation Method */
  argum.num_pc = 1;                              /* Number of principal components for PCA*/
  argum.roi = 0;                                 /* Argument for Region of interest */
  argum.rep_ts = 0;                              /* Argument for selecting represntative time series */
  argum.dset = "Default dataset";                              /* Dataset */
  argum.voxel_x = 0.0;
  argum.voxel_y = 0.0;
  argum.voxel_z = 0.0;           /* x,y,z coordinates of single voxel */
  argum.cube_dim = 0.0;                            /* side/2 of cube ROI */
  argum.cuboid_x = 0.0;
  argum.cuboid_y = 0.0;
  argum.cuboid_z = 0.0;        /* length/2, breadth/2, height/2 of cuboid ROI */
  argum.ellipse_rx = 0.0;
  argum.ellipse_ry = 0.0;
  argum.ellipse_rz = 0.0;  /* rx/2,ry/2,rz/2 for ellipse ROI */
  argum.sphere_r = 0.0;                            /* Radius of sphere ROI */
  argum.atlas_reg = 0;                        /* Atlas region number */
}

int main(int argc, char * argv[])
{
  
  long_index = 0;
  init_args();
  parse_long(argc, argv);
  printf("Correlation method is %d\n",argum.correlation_method);
  print_args();
  return 0;
}





