//****************************************************************
//* This file is part of the AsFem framework
//* A Simple Finite Element Method program (AsFem)
//* All rights reserved, Yang Bai @ CopyRight 2021
//* https://github.com/yangbai90/AsFem.git
//* Licensed under GNU GPLv3, please see LICENSE for details
//* https://www.gnu.org/licenses/gpl-3.0.en.html
//****************************************************************
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++ Author : Yang Bai
//+++ Date   : 2021.01.18
//+++ Purpose: implement the residual and jacobian for User-Defined-element
//+++          1) dc/dt=div(D*grad(c))
//+++          2) div(\Sigma)=0
//+++          this is used for testing purpose
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "ElmtSystem/BulkElmtSystem.h"

void BulkElmtSystem::User3Elmt(const FECalcType &calctype, const int &nDim, const int &nNodes, const int &nDofs,
                               const double &t, const double &dt, const double (&ctan)[2], const Vector3d &gpCoords,
                               const vector<double> &gpU, const vector<double> &gpV, const vector<Vector3d> &gpGradU,
                               const vector<Vector3d> &gpGradV, const double &test, const double &trial,
                               const Vector3d &grad_test, const Vector3d &grad_trial,
                               const ScalarMateType &ScalarMaterials, const VectorMateType &VectorMaterials,
                               const Rank2MateType &Rank2Materials, const Rank4MateType &Rank4Materials,
                               vector<double> &gpHist, vector<double> &gpHistOld, vector<double> &gpProj,
                               MatrixXd &localK, VectorXd &localR) {
    //*******************************************************
    //*** to get rid of the warning for unused variables  ***
    //*** for normal users, you dont need to do this      ***
    //*******************************************************
    if(nDim||nNodes||nDofs||t||dt||ctan[0]||gpCoords(1)||gpU.size()||gpV.size()||
       gpGradU.size()||gpGradV.size()||test||trial||grad_test(1)||grad_trial(1)||
       ScalarMaterials.size()||VectorMaterials.size()||Rank2Materials.size()||Rank4Materials.size()||
       gpHist.size()||gpHistOld.size()||gpProj.size()){}

    // Dofs: C -->1
    //       ux-->2
    //       uy-->3
    //       uz-->4
    switch (calctype){
        case FECalcType::ComputeResidual:
            // For R_d
            localR(1)=gpGradU[1]*grad_test;
            // For R_d
            localR(2)=gpGradU[2]*grad_test;
            break;
        case FECalcType::ComputeJacobian:
            // K_c,c
            localK(1,1)=grad_trial*grad_test*ctan[0];
            localK(2,2)=grad_trial*grad_test*ctan[0];
            break;
        case FECalcType::InitHistoryVariable:
            fill(gpHist.begin(),gpHist.end(),0.0);
            break;
        case FECalcType::UpdateHistoryVariable:
            gpHistOld=gpHist;
            break;
        case FECalcType::Projection:
            gpProj[0]=ScalarMaterials.at("vonMises");
            gpProj[1]=gpGradU[1](1);
            gpProj[2]=gpGradU[1](2);
            gpProj[3]=gpGradU[1](3);
            break;
        default:
            MessagePrinter::PrintErrorTxt("unsupported FEM calculation type in User1 element");
            MessagePrinter::AsFem_Exit();
            break;
    }
}