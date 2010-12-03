


class Surface
{
	MemoryFrame* m_pBufferX;
	MemoryFrame* m_pBufferY;
	MemoryFrame* m_pBufferZ;
	MemoryFrame* m_pIndexBuffer;
	Texture* m_pTexture;
public:

	int GetTextureID()
	{
		return m_pTexture->GetTextureID();
	}

	Surface()
	{}

	void Load(	MemoryFrame* pBufferX,
				MemoryFrame* pBufferY, 
				MemoryFrame* pBufferZ,
				MemoryFrame* pIndexBuffer;
				Texture* pTexture) 
				{
					m_pBufferX=pBufferX;
					m_pBufferY=m_pBufferY;
					m_pBufferZ=pBufferZ;
					m_pIndexBuffer=pIndexBuffer;
					m_pTexture=pTexture;
				}
};


