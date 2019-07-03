/*
 * Created by SharpDevelop.
 * User: Pwn
 * Date: 7/3/2019
 * Time: 5:50 PM
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Drawing;


namespace SteganographyEditor
{
	class SteganographyHelper
	{
		public enum State
    	{
	        Hiding,
	        Filling_With_Zeros
    	};
		
		public static Bitmap embedText(string text, Bitmap bmp)
		{
			State state = State.Hiding;
			// holds the index of the character that is being hidden
			int charIndex = 0;
			// holds the value of the character converted to integer
			int charValue = 0;
			//holds the value of (R G B) that is beeing processed
			int pixelElementIndex = 0;
			//trails of zeros added when finishing the process
			int zeros = 0;
			//hold the pixel elements
			int R=0,G=0,B=0;
			for(int i =0;i < bmp.Height;i++)
			{
				for(int j = 0;j < bmp.Width;j++)
				{
					// holds the pixel that is currently being processed
					Color pixel = bmp.GetPixel(j,i);
					
					// now, clear the least significant bit (LSB) from each pixel element
					R = pixel.R - pixel.R % 2;
					G = pixel.G - pixel.G % 2;
					B = pixel.B - pixel.B % 2;
					
					for(int n = 0; n < 3; n++)
					{
						if(pixelElementIndex % 8 == 0)
						{
							// check if the whole process has finished
                        	// we can say that it's finished when 8 zeros are added
                        	if(state == State.Filling_With_Zeros && zeros == 8)
                        	{
                        		//apply the last pixel on the image
	                            // even if only a part of its elements have been affected
	                            if ((pixelElementIndex - 1) % 3 < 2)
	                            {
	                                bmp.SetPixel(j, i, Color.FromArgb(R, G, B));
	                            }

	                            // return the bitmap with the text hidden in
	                            return bmp;
                        	}
						}
						// check if all characters has been hidden
                        if (charIndex >= text.Length)
                        {
                            // start adding zeros to mark the end of the text
                            state = State.Filling_With_Zeros;
                        }
                        else
                        {
                            // move to the next character and process again
                            charValue = text[charIndex++];
                        }
					}
					
					// check which pixel element has the turn to hide a bit in its LSB
					switch(pixelElementIndex % 3)
					{
						case 0:
						{
							if(state == State.Hiding)
							{
								// the rightmost bit in the character will be (charValue % 2)
                                // to put this value instead of the LSB of the pixel element
                                R += charValue % 2;

                               	// removes the added rightmost bit of the character
                               	// such that next time we can reach the next one
                                charValue /= 2;
							}
						}break;
						case 2:
                            {
                                if (state == State.Hiding)
                                {
                                    B += charValue % 2;

                                    charValue /= 2;
                                }

                                bmp.SetPixel(j, i, Color.FromArgb(R, G, B));
                            } break;
                    }

                    pixelElementIndex++;

                    if (state == State.Filling_With_Zeros)
                    {
                        // increment the value of zeros until it is 8
                        zeros++;
                    }
				}
			}
			return bmp;
		}

		public static void Main(string[] args)
		{
			
			Console.WriteLine("Hello World!");
			Bitmap image1 = new Bitmap(@"C:\Users\Pwn\Desktop\BMP.bmp" , true);

			// TODO: Implement Functionality Here
			embedText("vlad",image1);
			Console.Write("Press any key to continue . . . ");
			Console.ReadKey(true);
			
		}
	}
}