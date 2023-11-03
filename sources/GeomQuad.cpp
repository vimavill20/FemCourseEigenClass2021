/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "GeomQuad.h"

GeomQuad::GeomQuad() {
}

GeomQuad::~GeomQuad() {
}

GeomQuad::GeomQuad(const GeomQuad &copy) {
    fNodeIndices = copy.fNodeIndices;
}

GeomQuad& GeomQuad::operator=(const GeomQuad& copy) {
    fNodeIndices = copy.fNodeIndices;
    return *this;
}

void GeomQuad::Shape(const VecDouble &xi, VecDouble &phi, MatrixDouble &dphi) {
    
    if(xi.size() != Dimension || phi.size() != nCorners || dphi.rows() != Dimension || dphi.cols() != nCorners) DebugStop();
    phi.resize(4);
    dphi.resize(2,4);
    double csi=xi[0];
    double eta=xi[1];
    
    phi[0] = 0.25 * (1. - csi) * (1. - eta);
    dphi(0,0)= 0.25 * (-1. + eta);
    dphi(1,0)=0.25 * (-1. + csi);
    phi[1] = 0.25 * (1. + csi) * (1. - eta);
    dphi(0,1)=0.25 * (1. - eta);
    dphi(1,1)= 0.25 * (-1. - csi);
    phi[2] = 0.25 * (1. + csi) * (1. + eta);
    dphi(0,2)=0.25 * (1. + eta);
    dphi(1,2)=0.25 * (1. + csi);
    phi[3] = 0.25 * (1. - csi) * (1. + eta);
    dphi(0,3)=0.25 * (-1. - eta);
    dphi(1,3)= 0.25 * (1. - csi);
    
}

void GeomQuad::X(const VecDouble &xi, MatrixDouble &NodeCo, VecDouble &x) {
    
    if(xi.size() != Dimension) DebugStop();
    if(x.size() < NodeCo.rows()) DebugStop();
    if(NodeCo.cols() != nCorners) DebugStop();
    int nrow = NodeCo.rows();
    int ncol = NodeCo.cols();
    if (x.size() < nrow) {
        x.resize(2);
    }

    x.setZero();

        VecDouble phi(nCorners);
        MatrixDouble dphi(Dimension, nCorners);

        Shape(xi, phi, dphi);
}

void GeomQuad::GradX(const VecDouble &xi, MatrixDouble &NodeCo, VecDouble &x, MatrixDouble &gradx) {
    std::cout << "\nPLEASE IMPLEMENT ME\n" << __PRETTY_FUNCTION__ << std::endl;
    if(xi.size() != Dimension) DebugStop();
    if(x.size() != NodeCo.rows()) DebugStop();
    if(NodeCo.cols() != nCorners) DebugStop();
    DebugStop();
}

void GeomQuad::SetNodes(const VecInt &nodes) {
    if(nodes.size() != nCorners) DebugStop();
    fNodeIndices = nodes;
}

void GeomQuad::GetNodes(VecInt &nodes) const{
    nodes = fNodeIndices;
}

int GeomQuad::NodeIndex(int node) const {
    return fNodeIndices[node];
}

int GeomQuad::NumNodes() {
    return nCorners;
}

GeoElementSide GeomQuad::Neighbour(int side) const {
    return fNeighbours[side];
}

void GeomQuad::SetNeighbour(int side, const GeoElementSide &neighbour) {
    fNeighbours[side] = neighbour;
}
