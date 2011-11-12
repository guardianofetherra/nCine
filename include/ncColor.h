#ifndef CLASS_NCCOLOR
#define CLASS_NCCOLOR

#include <cstdio> // for printf()

/// A 32 bit color
class ncColor
{
public:
	GLubyte m_color[4];

	/// Default constructor (white color)
	ncColor() { Set(255, 255, 255, 255); }
	/// Four components constructor (unsigned char)
	ncColor(unsigned char ucR, unsigned char ucG, unsigned char ucB, unsigned char ucA)
		{ Set(ucR, ucG, ucB, ucA); }
	/// Three components constructor (unsigned char)
	ncColor(unsigned char ucR, unsigned char ucG, unsigned char ucB)
		{ Set(ucR, ucG, ucB); }
	/// Four components constructor (normalized float)
	ncColor(float fR, float fG, float fB, float fA)
		{ SetF(fR, fG, fB, fA); }
	/// Three components constructor (normalized float)
	ncColor(float fR, float fG, float fB)
		{ SetF(fR, fG, fB); }

	/// Getter for the red component of the color
	inline GLubyte R() const { return m_color[0]; }
	/// Getter for the green component of the color
	inline GLubyte G() const { return m_color[1]; }
	/// Getter for the blue component of the color
	inline GLubyte B() const { return m_color[2]; }
	/// Getter for the alpha component of the color
	inline GLubyte A() const { return m_color[3]; }
	/// Getter for the color vector, useful with glColor4ub
	inline const GLubyte* Vector() const { return m_color; }
	/// Flaot getter for the red component of the color
	inline float fR() const { return m_color[0]/255.0f; }
	/// Float getter for the green component of the color
	inline float fG() const { return m_color[1]/255.0f; }
	/// Float getter for the blue component of the color
	inline float fB() const { return m_color[2]/255.0f; }
	/// Float getter for the alpha component of the color
	inline float fA() const { return m_color[3]/255.0f; }

	/// Set four color elements (unsigned char)
	inline void Set(unsigned char ucR, unsigned char ucG, unsigned char ucB, unsigned char ucA)
	{
		m_color[0] = ucR;
		m_color[1] = ucG;
		m_color[2] = ucB;
		m_color[3] = ucA;
	}

	/// Set three color elements (unsigned char)
	inline void Set(unsigned char ucR, unsigned char ucG, unsigned char ucB)
	{
		Set(ucR, ucG, ucB, 255);
	}

	/// Set four color elements (unsigned char vector)
	inline void Setv(unsigned char vColor[4])
	{
		Set(vColor[0], vColor[1], vColor[2], vColor[3]);
	}

	/// Set four color elements (normalized float)
	inline void SetF(float fR, float fG, float fB, float fA)
	{
		m_color[0] = fR > 1.0f ? 255 : fR * 255;
		m_color[1] = fG > 1.0f ? 255 : fG * 255;
		m_color[2] = fB > 1.0f ? 255 : fB * 255;
		m_color[3] = fA > 1.0f ? 255 : fA * 255;
	}

	/// Set three color elements (normalized float)
	inline void SetF(float fR, float fG, float fB) {
		SetF(fR, fG, fB, 1.0f);
	}

	/// Set four color elements (float vector)
	inline void SetFv(float fvColor[4])
	{
		SetF(fvColor[0], fvColor[1], fvColor[2], fvColor[3]);
	}

	/// Equality operator
	inline bool operator==(const ncColor& color) const {
		return (R() == color.R() && G() == color.G() &&
				B() == color.B() && A() == color.A());
	}

	inline void Print() const {
		printf("(%u, %u, %u, %u)", R(), G(), B(), A());
	}
};

#endif