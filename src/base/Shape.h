#ifndef SHAPE_H
#define SHAPE_H

#include "Include.h"
#include "SVGPlot.h"
using namespace std;

class Shape {
    public:
        Shape(SVGPlot& plot) : _plot(plot) {}
        ~Shape() {}
        // double ctrX() const { return _center.first; }
        // double ctrY() const { return _center.second; }
        virtual double ctrX() { double ctrX; return ctrX; }
        virtual double ctrY() { double ctrY; return ctrY; }
        virtual void print() {}
        virtual void plot(size_t colorId, size_t layId) {}
        virtual double maxX() { double maxX; return maxX; }
        virtual double minX() { double minX; return minX; }
        virtual double maxY() { double maxY; return maxY; }
        virtual double minY() { double minY; return minY; }
        virtual double boxH() { return maxX() - minX(); }
        virtual double boxW() { return maxY() - minY(); }
        virtual double bPolygonX(size_t vtxId) { double bPolygonX; return bPolygonX;}
        virtual double bPolygonY(size_t vtxId) { double bPolygonY; return bPolygonY;}
        virtual size_t numBPolyVtcs() { size_t numBPolyVtcs; return numBPolyVtcs;}
        virtual bool enclose(double x, double y);
    protected:
        SVGPlot& _plot;
        // pair<double, double> _center;
};

class Polygon : public Shape {
    public:
        Polygon(vector< pair<double, double> > vVtx, SVGPlot& plot) : _vVtx(vVtx), Shape(plot) {}
        ~Polygon(){}
        size_t numVtcs() const { return _vVtx.size(); }
        double vtxX(size_t vtxIdx) const { return _vVtx[vtxIdx].first;}
        double vtxY(size_t vtxIdx) const { return _vVtx[vtxIdx].second;}
        double ctrX() {
            double sumX = 0;
            for (size_t vtxId = 0; vtxId < _vVtx.size(); ++ vtxId) {
                sumX += _vVtx[vtxId].first;
            }
            return sumX / _vVtx.size();
        }
        double ctrY() {
            double sumY = 0;
            for (size_t vtxId = 0; vtxId < _vVtx.size(); ++ vtxId) {
                sumY += _vVtx[vtxId].second;
            }
            return sumY / _vVtx.size();
        }
        void print() {
            cerr << "Polygon {vVtx= ";
            for (size_t vtxId = 0; vtxId < _vVtx.size(); ++ vtxId) {
                cerr << "(" << _vVtx[vtxId].first << " " << _vVtx[vtxId].second << "), ";
            }
            cerr << "}" << endl;
        }
        void plot(size_t colorId, size_t layId) {
            _plot.drawPolygon(_vVtx, colorId, layId);
        }
        double maxX() {
            double maxX = _vVtx[0].first;
            for (size_t vtxId = 1; vtxId < _vVtx.size(); ++ vtxId) {
                if (_vVtx[vtxId].first > maxX) {
                    maxX = _vVtx[vtxId].first;
                }
            }
            return maxX;
        }
        double minX() {
            double minX = _vVtx[0].first;
            for (size_t vtxId = 1; vtxId < _vVtx.size(); ++ vtxId) {
                if (_vVtx[vtxId].first < minX) {
                    minX = _vVtx[vtxId].first;
                }
            }
            return minX;
        }
        double maxY() {
            double maxY = _vVtx[0].second;
            for (size_t vtxId = 1; vtxId < _vVtx.size(); ++ vtxId) {
                if (_vVtx[vtxId].second > maxY) {
                    maxY = _vVtx[vtxId].second;
                }
            }
            return maxY;
        }
        double minY() {
            double minY = _vVtx[0].second;
            for (size_t vtxId = 1; vtxId < _vVtx.size(); ++ vtxId) {
                if (_vVtx[vtxId].second < minY) {
                    minY = _vVtx[vtxId].second;
                }
            }
            return minY;
        }
        double bPolygonX(size_t vtxId) { return _vVtx[vtxId].first; }
        double bPolygonY(size_t vtxId) { return _vVtx[vtxId].second; }
        size_t numBPolyVtcs() { return _vVtx.size(); }
    private:
        vector< pair<double, double> > _vVtx;
};

class Circle : public Shape {
    public:
        Circle(double ctrX, double ctrY, double radius, SVGPlot& plot) : _radius(radius), Shape(plot) {
            _ctr = make_pair(ctrX, ctrY);
        }
        ~Circle() {}
        double ctrX() { return _ctr.first; }
        double ctrY() { return _ctr.second; }
        double radius() const { return _radius; }
        void print() {
            cerr << "Circle {center=(" << _ctr.first << " " << _ctr.second << "), radius=" << _radius << "}" << endl;
        }
        void plot(size_t colorId, size_t layId) {
            _plot.drawCircle(_ctr.first, _ctr.second, _radius, colorId, layId);
        }
        double maxX() { return _ctr.first + _radius; }
        double minX() { return _ctr.first - _radius; }
        double maxY() { return _ctr.second + _radius; }
        double minY() { return _ctr.second - _radius; }
        double bPolygonX(size_t vtxId) {
            if (vtxId == 0 || vtxId == 3) return minX();
            else return maxX();
        }
        double bPolygonY(size_t vtxId) {
            if (vtxId == 0 || vtxId == 1) return minY();
            else return maxY();
        }
        size_t numBPolyVtcs() { return 4; }
        bool enclose(double x, double y) {
            double dist = sqrt(pow(x-_ctr.first,2) + pow(y-_ctr.second,2));
            return (dist < _radius);
        }
    private:
        pair<double, double> _ctr;
        double _radius;
};

class Node : public Shape {
    public:
        Node(double ctrX, double ctrY, SVGPlot& plot) : Shape(plot) {
            _ctr = make_pair(ctrX, ctrY);
        }
        ~Node() {}
        double ctrX() { return _ctr.first; }
        double ctrY() { return _ctr.second; }
        void print() {
            cerr << "Node {center=(" << _ctr.first << " " << _ctr.second << ")}" << endl;
        }
        void plot(size_t colorId, size_t layId) {
            _plot.drawCircle(_ctr.first, _ctr.second, 2, colorId, layId);
        } 
        double maxX() { return _ctr.first; }
        double minX() { return _ctr.first; }
        double maxY() { return _ctr.second; }
        double minY() { return _ctr.second; }
        double bPolygonX(size_t vtxId) { return _ctr.first; }
        double bPolygonY(size_t vtxId) { return _ctr.second; }
        size_t numBPolyVtcs() { return 1; }
        // size_t layId() { return _layId; }
        // void setLayId(size_t layId) { _layId = layId; }
    private:
        pair<double, double> _ctr;
        // size_t _layId;
};

class Trace : public Shape {
    public:
        Trace(Node* sNode, Node* tNode, double width, SVGPlot& plot) : _sNode(sNode), _tNode(tNode), _width(width), Shape(plot) {}
        ~Trace() {}
        Node* sNode() { return _sNode; }
        Node* tNode() { return _tNode; }
        double width() const { return _width; }
        void print() {
            cerr << "Trace {" << endl;
            cerr << "sNode=";
            _sNode->print();
            cerr << ", tNode=";
            _tNode->print();
            cerr << ", width=" << _width << endl;
            cerr << "}" << endl;
        }

        void plot(size_t colorId, size_t layId) {
            _plot.drawLine(_sNode->ctrX(), _sNode->ctrY(), _tNode->ctrX(), _tNode->ctrY(), colorId, layId, _width);
        }

        double maxX() { return (_sNode->ctrX() > _tNode->ctrX()) ? _sNode->ctrX() : _tNode->ctrX(); }
        double minX() { return (_sNode->ctrX() < _tNode->ctrX()) ? _sNode->ctrX() : _tNode->ctrX(); }
        double maxY() { return (_sNode->ctrY() > _tNode->ctrY()) ? _sNode->ctrY() : _tNode->ctrY(); }
        double minY() { return (_sNode->ctrY() < _tNode->ctrY()) ? _sNode->ctrY() : _tNode->ctrY(); }
        double bPolygonX(size_t vtxId) {
            double orgX;
            if (vtxId == 0 || vtxId == 1) orgX = _sNode->ctrX();
            else orgX = _tNode->ctrX();
            double offset;
            if (vtxId == 0 || vtxId == 3) offset = -0.5 * _width;
            else offset = 0.5 *_width;
            return orgX + offset * (_tNode->ctrY() - _sNode->ctrY()) / 
                                    sqrt(pow(_tNode->ctrX() - _sNode->ctrX(), 2) + pow(_tNode->ctrY() - _sNode->ctrY(), 2));
        }
        double bPolygonY(size_t vtxId) {
            double orgY;
            if (vtxId == 0 || vtxId == 1) orgY = _sNode->ctrY();
            else orgY = _tNode->ctrY();
            double offset;
            if (vtxId == 0 || vtxId == 3) offset = -0.5 * _width;
            else offset = 0.5 *_width;
            return orgY + offset * -(_tNode->ctrX() - _sNode->ctrX()) / 
                                    sqrt(pow(_tNode->ctrX() - _sNode->ctrX(), 2) + pow(_tNode->ctrY() - _sNode->ctrY(), 2));
        }
        size_t numBPolyVtcs() { return 4; }
    private:
        Node* _sNode;
        Node* _tNode;
        double _width;
};

#endif