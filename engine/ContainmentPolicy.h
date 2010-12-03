/*
    High Adventure 3D Game Engine
    Copyright (C) 2002  Rodney Degracia

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	rodney_degracia(at)elitefrontier.com

	Rodney Degracia
	316 Independence Ave. SE 
	Washington DC 2003
*/

#pragma once



template <typename BoundingSphereTrait>
class BoundingSphere	//Note 'Bounding' prefix suggests containment
{

protected:
	BoundingSphere(){}

public:

	typename BoundingSphereTrait::BoundingRegion ComputeContainingAxisAlignedBox(typename BoundingSphereTrait::MeshVertexBuffer& mvb)
	{
		typename BoundingSphereTrait::VertexComponent minx=typename BoundingSphereTrait::VertexComponent();
		typename BoundingSphereTrait::VertexComponent miny=typename BoundingSphereTrait::VertexComponent();
		typename BoundingSphereTrait::VertexComponent minz=typename BoundingSphereTrait::VertexComponent();

		typename BoundingSphereTrait::VertexComponent maxx=typename BoundingSphereTrait::VertexComponent();
		typename BoundingSphereTrait::VertexComponent maxy=typename BoundingSphereTrait::VertexComponent();
		typename BoundingSphereTrait::VertexComponent maxz=typename BoundingSphereTrait::VertexComponent();

		typename BoundingSphereTrait::Count count=mvb.GetCount();
		for (size_t i=0; i<count;++i)
		{
			if((mvb.m_ArrayData.pXCoords[i])<minx) minx=(mvb.m_ArrayData.pXCoords[i]);
			else if(mvb.m_ArrayData.pXCoords[i]>maxx) maxx=mvb.m_ArrayData.pXCoords[i];

			if(mvb.m_ArrayData.pYCoords[i]<miny) miny=mvb.m_ArrayData.pYCoords[i];
			else if(mvb.m_ArrayData.pYCoords[i]>maxy) maxy=mvb.m_ArrayData.pYCoords[i];

			if(mvb.m_ArrayData.pZCoords[i]<minz) minz=mvb.m_ArrayData.pZCoords[i];
			else if(mvb.m_ArrayData.pZCoords[i]>maxz) maxz=mvb.m_ArrayData.pZCoords[i];
		}

		typename BoundingSphereTrait::Vertex minimum(minx,miny,minz), maximum(maxx,maxy,maxz);
 		typename BoundingSphereTrait::Point center(minimum+maximum/2);
		typename BoundingSphereTrait::Vertex diagonal(((maximum)-(minimum))/2.0f);
		typename BoundingSphereTrait::Scalar radius=diagonal.Magnitude()/2;

#ifdef _DEBUG
		//Draw the diagonal of the AABox
		glPushMatrix();
		glColor3f(0,0,1);
		glBegin(GL_LINES);
		glVertex3f(minimum.x,minimum.y,minimum.z);
		glVertex3f(maximum.x,maximum.y,maximum.z);
		glEnd();
		glPopMatrix();
#endif
	
		typename BoundingSphereTrait::BoundingRegion br(center,radius);
		return br;
	}


	//Dual Merge
	typename BoundingSphereTrait::BoundingRegion MergeWith(typename const BoundingSphereTrait::BoundingRegion& br1,typename const BoundingSphereTrait::BoundingRegion& br2)
	{
		typename const BoundingSphereTrait::BoundingRegion& C0=br1;
		typename const BoundingSphereTrait::BoundingRegion& C1=br2;

		typename BoundingSphereTrait::Point CenterDiff((C1.m_C.x-C0.m_C.x),
													   (C1.m_C.y-C0.m_C.y),
													   (C1.m_C.z-C0.m_C.z));

		float L=CenterDiff.Magnitude();


		float radiusdiff=(C1.m_R-C0.m_R);
		if (((radiusdiff)*(radiusdiff))>=(L*L))
		{
			if (C1.m_R>=C0.m_R)
			{
				typename BoundingSphereTrait::BoundingRegion br(C1.m_C,C1.m_R);

#ifdef _DEBUG
				glBegin(GL_LINES);
				glColor3f(1.0f,1.0f,1.0f);
				glVertex3f(br.m_C.x+br.m_R,br.m_C.y,br.m_C.z);
				glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
				glVertex3f(br.m_C.x,br.m_C.y+br.m_R,br.m_C.z);
				glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
				glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z+br.m_R);
				glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
				glEnd();
#endif

				return (br);
			}
			else
			{
				typename BoundingSphereTrait::BoundingRegion br(C0.m_C,C0.m_R);

#ifdef _DEBUG
				glBegin(GL_LINES);
				glColor3f(1.0f,1.0f,1.0f);
				glVertex3f(br.m_C.x+br.m_R,br.m_C.y,br.m_C.z);
				glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
				glVertex3f(br.m_C.x,br.m_C.y+br.m_R,br.m_C.z);
				glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
				glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z+br.m_R);
				glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
				glEnd();
#endif

				return (br);
			}
		}
		else
		{
			typename BoundingSphereTrait::ColumnVector C(C0.m_C+(((L+radiusdiff)/(2*L))*(CenterDiff)));
			typename BoundingSphereTrait::BoundingRegion br(C,((L+C1.m_R+C0.m_R)/2));

#ifdef _DEBUG
			glBegin(GL_LINES);
			glColor3f(1.0f,1.0f,1.0f);
			glVertex3f(br.m_C.x+br.m_R,br.m_C.y,br.m_C.z);
			glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
			glVertex3f(br.m_C.x,br.m_C.y+br.m_R,br.m_C.z);
			glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
			glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z+br.m_R);
			glVertex3f(br.m_C.x,br.m_C.y,br.m_C.z);
			glEnd();
#endif

			return (br);
		}
	}


};