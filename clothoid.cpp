template <class T, class Real, class R>
void simpson_integral(T f, Real a, Real b, R *r) {
    Real mul = (b - a) * static_cast<Real>(1.0 / 6.0);
    *r = mul * (f(a) + static_cast<Real>(4.0) * f((a + b) * static_cast<Real>(0.5)) + f(b));
}

float phi(float phi0, float phiV, float phiU, float S) {
    return phi0 + phiV * S + phiU * S * S;
}

std::complex<float> slope_f(float phi0, float phiV, float phiU, float S) {
    std::complex<float> j(0.0f, 1.0f);
    return std::exp(j * phi(phi0, phiV, phiU, S));
}

struct Slope {
    float phi0;
    float phiV;
    float phiU;
    std::complex<float> operator()(float S) {
        return slope_f(phi0, phiV, phiU, S);
    }
};

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);

    ofSetMatrixMode(OF_MATRIX_PROJECTION);
    ofPushMatrix();
    ofLoadIdentityMatrix();
    ofSetMatrixMode(OF_MATRIX_MODELVIEW);
    ofPushMatrix();
    ofLoadIdentityMatrix();
    ofScale(1, -1, 1);

    float s = 0.5f;
    glScalef(s, s, s);

    float h = _hValue;
    float phi0 = _phi0Value;
    float phiV = _phiVValue;
    float phiU = _phiUValue;

    int n = 1000;
    float stepS = 1.0f / n;
    std::vector<ofPoint> points;

    Slope slope;
    slope.phi0 = phi0;
    slope.phiV = phiV;
    slope.phiU = phiU;

    std::complex<float> P_Vector;
    for(int i = 0 ; i < n ; ++i) {
        float S = stepS * i;

        std::complex<float> r;
        simpson_integral(slope, S, S + stepS, &r);
        P_Vector += r;

        float x = P_Vector.real();
        float y = P_Vector.imag();
        points.push_back(h * ofVec2f(x, y));
    }

    ofPolyline polyline(points);
    ofSetLineWidth(2);
    polyline.draw();
    ofSetLineWidth(1);

    ofSetMatrixMode(OF_MATRIX_PROJECTION);
    ofPopMatrix();
    ofSetMatrixMode(OF_MATRIX_MODELVIEW);
    ofPopMatrix();

    _gui.draw();
}