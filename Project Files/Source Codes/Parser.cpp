#include<iostream>
#include <vector>
#include<fstream>
#include<sstream>
#include"Parser.h"
#include"PinI.h"
#include"PinO.h"
#include"Cell.h"
#include<string>
#include<algorithm>

using namespace std;

void  Parser::ReadLib(string lib_name)
{
	fstream in;
	string s;
	string name;
	string direction;
	char k;
	string word="";
	string related_pin;
	bool enable=false;
	bool clk=false;
	bool cio=false;
	bool fax=false;
	int row=0;int col=0; int r; int l;
	in.open(lib_name.c_str());
	if(in.is_open())
	{
		while(!in.eof())
		{
			getline(in,s,'(');
			enable=false;
			cio=false;
			fax=false;
			if(s=="cell ")
			{
				Cell c;
				PinI p1;
				int x=0;
				getline(in,s,')'); c.nameC=s;
				getline(in,s,':');//rest of line, unneeded
				in>>c.area; //area
				getline(in,s,':');
				in>>c.C_Leakage_P;//cell_leakage_power
				do{ word="";
					getline(in,s);
					in>>s;
					for(int i=0; i<3; i++)
						word=word+s[i];
					s.erase(0,4);
					s.erase(s.length()-1,1);
					name=s;
					///////////
					do{
						in>>s; 
					}while(s!="direction");
					//for(int i=0; i<2; i++)
						in>>k;
					in>>direction;
					//////////
					direction.erase(direction.length()-1,1);
					if(direction=="input")
					{       PinI p;
							if(name=="EN")
								{
									p1.nameP=name+"1";
									p.nameP=name+"0";
									p1.EN=true;
									p.EN=true;
									enable=true;
									p1.CLK=false;
									p.CLK=false;
								}
					
							else
							{p.nameP=name; p.EN=false;}
							if(name=="CLK")
							{
								p1.nameP=name+"s";
								p.nameP=name+"h";
								p1.CLK=true;
								p.CLK=true;
								clk=true;
								cio=true;

							}
							else
							{p.CLK=false; clk=false;}
								do{
								in>>s; 
							}while(s!="capacitance");
								in>>k;
							in>>p.capacitance;
							//rise_capacitance
							do{
								in>>s; 
							}while(s!="rise_capacitance");
								in>>k;
							in>>p.rise_capacitance;
							//fall_capacitance
							do{
								in>>s; 
							}while(s!="fall_capacitance");
								in>>k;
							in>>p.fall_capacitance;
							c.pins.push_back(p); //push to pins vector
							
							if(enable || clk ||c.nameC=="DFFNEGX1" ||c.nameC=="DFFPOSX1")///////if EN
							{
								getline(in,s,';');
								if(enable || clk)
									c.pins.push_back(p1); 
								
								do{
									in>>s; 
								}while(s!="internal_power()"); getline(in,s);//rest of line
								in>>s;
									string temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										if(temp=="rise_power")
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											row=char_int(k);
											k=s.at(s.length()-3);
											col=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[c.pins.size()-2].risePEN_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');
											if(row>1)
												{for(int n=0; n<row;n++)//index2
												{
													in>>z;
													c.pins[c.pins.size()-2].risePEN_index2.push_back(z);
													in>>k;
												}getline(in,s,'"');}
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}
												if(m==row-1)
													getline(in,s,'}');
												else 
													getline(in,s,'"');
												c.pins[c.pins.size()-2].risePEN.push_back(values);
											}
											in>>s;
											temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										}
										if(temp=="fall_power")
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											row=char_int(k);
											k=s.at(s.length()-3);
											col=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[c.pins.size()-2].fallPEN_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');
											if(row>1)
												{for(int n=0; n<row;n++)//index2
												{
													in>>z;
													c.pins[c.pins.size()-2].fallPEN_index2.push_back(z);
													in>>k;
												}getline(in,s,'"');}
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}
												if(m==row-1)
													getline(in,s);
												else 
													getline(in,s,'"');
												c.pins[c.pins.size()-2].fallPEN.push_back(values);
											}
								
							}
					}
					if(!clk && cio)
					{bool flag=true;
					
						for(int q=0; q<c.pins.size()-1;q++)
							{
						do{
								in>>s; 
							}while(s!="timing()"); getline(in,s);//rest of line
					/////////////////////////////////
					in>>s; 
							if(s=="related_pin")
							{
								in>>k; in>>k; getline(in,s,'"');  int i;
								for(i=0; c.pins.size(); i++)
									{
									if(c.pins[i].nameP==(s+'h') && flag) 
											{
												flag=false; break;
									}
									else if( c.pins[i].nameP==(s+'s'))
									{
										break;
									}
								}
								getline(in,s);//rest of line
								in>>s;
								if (s=="timing_type") //timing type if exists
										{
											in>>k; getline(in,s); s.erase(0,1);s.erase(s.length()-1,1); c.pins[i].timing_type=s;
											in>>s;
										}
									
										string temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										if(temp=="rise_constraint")
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											col=char_int(k);
											k=s.at(s.length()-3);
											row=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<row;n++)//index2
												{
													in>>z;
													c.pins[i].riseCLK_index2.push_back(z);
													in>>k;
												}
											getline(in,s,'"');
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[i].riseCLK_index1.push_back(z);
												in>>k;
											}
													getline(in,s,'"');
											
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}
												if(m==row-1)
													getline(in,s,'}');
												else 
													getline(in,s,'"');
												c.pins[i].riseCLK.push_back(values);
											}
											in>>s;
											temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										}
										if(temp=="fall_constraint" )
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											col=char_int(k);
											k=s.at(s.length()-3);
											row=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											
											for(int n=0; n<row;n++)//index2
											{
												in>>z;
												c.pins[i].fallCLK_index2.push_back(z);
												in>>k;
												}
											getline(in,s,'"');
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[i].fallCLK_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');

											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);

													in>>k;
												}

												if(m==row-1)
													getline(in,s,'}');
												else 
													getline(in,s,'"');
												c.pins[i].fallCLK.push_back(values);
											}
										}
								
					/////////////////////////////////
										}
								
							}
					flag=false;
					getline(in,s);
					getline(in,s);
					}
					else
						getline(in,s,'}');
					if(enable)
					{
						getline(in,s);
						getline(in,s);
					}
					else if(clk)
						{
							getline(in,s); getline(in,s);
							in>>s; if(s=="min_pulse_width_high"){in>>k; in>>p.MPWH;  getline(in,s);}
							in>>s; if(s=="min_pulse_width_low"){in>>k; in>>p.MPWL;  getline(in,s);}
						}
					}
					if(direction=="output"||direction=="inout")
					{   
						PinO o;
						o.nameO=name;
							do{
								in>>s; 
							}while(s!="capacitance");
								in>>k;
							in>>o.capacitance;
							//rise_capacitance
							do{
								in>>s; 
							}while(s!="rise_capacitance");
								in>>k;
							in>>o.rise_capacitance;
							//fall_capacitance
							do{
								in>>s; 
							}while(s!="fall_capacitance");
								in>>k;
							in>>o.fall_capacitance;
							//max_capacitance
							do{
								in>>s; 
							}while(s!="max_capacitance");
								in>>k;
							in>>o.max_capacitance;
							c.out.push_back(o);bool flag=true;
							for(int q=0; q<c.pins.size();q++)
							{
								do{
								in>>s; 
							}while(s!="timing()"); getline(in,s);//rest of line
							in>>s; 
							if(s=="related_pin")
							{
								in>>k; in>>k; getline(in,s,'"');  int i;
								for(i=0; c.pins.size(); i++)
									{
									if(c.pins[i].nameP==(s+'0') && flag) 
											{
												flag=false; break;
									}
									else if( c.pins[i].nameP==(s+'1'))
										break;
									else if(c.pins[i].nameP==(s+'h')) //CLK
													{
														q=c.pins.size()-1; break;
											}
									else if(c.pins[i].nameP==s)
											break;
								}
								 getline(in,s);//rest of line
								in>>s;
								if(s=="timing_sense") //timing sense
								{
									in>>k; getline(in,s); s.erase(0,1); s.erase(s.length()-1,1);
									if(s=="positive_unate")
										c.pins[i].time_sense='+';
									else if(s=="negative_unate")
										c.pins[i].time_sense='-';
									else if(s=="non_unate")
										c.pins[i].time_sense='=';
									in>>s;
										if (s=="timing_type") //timing type if exists
										{
											in>>k; getline(in,s);  s.erase(0,1);s.erase(s.length()-1,1); c.pins[i].timing_type=s; 
											in>>s;
										}
									
										string temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										if(temp=="cell_rise" || temp=="cell_fall")
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											row=char_int(k);
											k=s.at(s.length()-3);
											col=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[i].riseC_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');
											if(row>1)
												{
													for(int n=0; n<row;n++)//index2
												{
													in>>z;
													c.pins[i].riseC_index2.push_back(z);
													in>>k;
												}
													getline(in,s,'"');}
											if(c.nameC=="DFFNEGX1" ||c.nameC=="DFFPOSX1")
											{
												int tm=row;
												row=col;
												col=tm;
											}
											
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}

												if(m==row-1)
													getline(in,s,'}');
												else 
													getline(in,s,'"');
												c.pins[i].riseC.push_back(values);
											}
											in>>s;
											temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										}
										if(temp=="rise_transition" || temp=="fall_transition")
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											row=char_int(k);
											k=s.at(s.length()-3);
											col=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[i].riseT_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');
											if(row>1)
											{	for(int n=0; n<row;n++)//index2
											{
												in>>z;
												c.pins[i].riseT_index2.push_back(z);
												in>>k;
											}
												getline(in,s,'"');}
											if(c.nameC=="DFFNEGX1" ||c.nameC=="DFFPOSX1")
											{
												int tm=row;
												row=col;
												col=tm;
											}
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}
												if(m==row-1)
													getline(in,s,'}');
												else 
													getline(in,s,'"');
												c.pins[i].riseT.push_back(values);
											}
											in>>s;
											temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										}
										if(temp=="cell_fall" ||temp=="cell_rise" )
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											row=char_int(k);
											k=s.at(s.length()-3);
											col=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[i].fallC_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');
											if(row>1)
												{for(int n=0; n<row;n++)//index2
											{
												in>>z;
												c.pins[i].fallC_index2.push_back(z);
												in>>k;
											}
												getline(in,s,'"');}
											if(c.nameC=="DFFNEGX1" ||c.nameC=="DFFPOSX1")
											{
												int tm=row;
												row=col;
												col=tm;
											}
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}

												if(m==row-1)
													getline(in,s,'}');
												else 
													getline(in,s,'"');
												c.pins[i].fallC.push_back(values);
											}
											in>>s;
											temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										}
										if(temp=="fall_transition" || temp=="rise_transition")
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											row=char_int(k);
											k=s.at(s.length()-3);
											col=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[i].fallT_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');
											if(row>1)
												{for(int n=0; n<row;n++)//index2
											{
												in>>z;
												c.pins[i].fallT_index2.push_back(z);
												in>>k;
												}getline(in,s,'"');}
											if(c.nameC=="DFFNEGX1" ||c.nameC=="DFFPOSX1")
											{
												int tm=row;
												row=col;
												col=tm;
											}
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}

												if(m==row-1)
													getline(in,s,'}');
												else 
													getline(in,s,'"');
												c.pins[i].fallT.push_back(values);
											}
										}
								

								}

							}
					     }flag=true;
						 for(int q=0; q<c.pins.size();q++)
							{
									do{
									in>>s; 
								}while(s!="internal_power()"); getline(in,s);//rest of line
								in>>s; 
								if(s=="related_pin")
								{
									 in>>k; in>>k; getline(in,s,'"');  int i;
									for(i=0; c.pins.size(); i++)
										{
											if(c.pins[i].nameP==(s+'0') && flag) //EN
													{
														flag=false; q=c.pins.size()-1; break;
											}
											else if(c.pins[i].nameP==(s+'h')) //CLK
													{
														q=c.pins.size()-1; break;
											}
											else if(c.pins[i].nameP==s)
												break;
									} getline(in,s);//rest of line
									in>>s;
									string temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										if(temp=="rise_power" ||temp=="fall_power")
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											row=char_int(k);
											k=s.at(s.length()-3);
											col=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[i].riseP_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');
											if(row>1)
												{for(int n=0; n<row;n++)//index2
												{
													in>>z;
													c.pins[i].riseP_index2.push_back(z);
													in>>k;
												}getline(in,s,'"');}
											if(c.nameC=="DFFNEGX1" ||c.nameC=="DFFPOSX1")
											{
												int tm=row;
												row=col;
												col=tm;
											}
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}

												if(m==row-1)
													getline(in,s,'}');
												else 
													getline(in,s,'"');
												c.pins[i].riseP.push_back(values);
											}
											in>>s;
											temp="";
										for(int i=0; s.at(i)!='('; i++)
											temp=temp+s.at(i);
										}
										if(temp=="fall_power" || temp=="rise_power")
										{
											s.erase(s.length()-1,1);
											k=s.at(s.length()-1);
											row=char_int(k);
											k=s.at(s.length()-3);
											col=char_int(k);
											getline(in,s,'"'); r=0; l=0;
											float z;
											for(int n=0; n<col;n++)//index1
											{
												in>>z;
												c.pins[i].fallP_index1.push_back(z);
												in>>k;
											}
											getline(in,s,'"');
											if(row>1)
												{for(int n=0; n<row;n++)//index2
											{
												in>>z;
												c.pins[i].fallP_index2.push_back(z);
												in>>k;
												}getline(in,s,'"');}
											if(c.nameC=="DFFNEGX1" ||c.nameC=="DFFPOSX1")
											{
												int tm=row;
												row=col;
												col=tm;
											}
											for(int m=0; m<row;m++)//2D table
											{
												vector<float>values;
												for(int n=0; n<col;n++)//2D table
												{
													in>>z;
													values.push_back(z);
													in>>k;
												}

												if(m==row-1)
													getline(in,s);
												else 
													getline(in,s,'"');
												c.pins[i].fallP.push_back(values);
											}
								}
						 }
					  }
					 if((c.nameC=="FAX1" ||c.nameC=="HAX1")&& !fax)
					  {
						  fax=true;
						  getline(in,s);
						  getline(in,s);
					  }
					 else if(c.nameC!="FAX1" ||fax)
					{ word="";
					  for(int j=0; j<5;j++)getline(in,s);
					  library.push_back(c);}
				    }
				}while(word=="pin");
				
			}
			
	   }
	}
	else
	{
		cout<<"ERROR!, Cannot open "<<lib_name<<" file!"<<endl;
	}
	in.close();
}
float Parser::Cell_Set_Hold(string cell, string pin, float index2, float index1, int b) //reversed indecies
{
	float value;
	PinI in=Return_Pin(cell,pin);
	float Ybefore, Yafter,Xbefore, Xafter;
	float TYbefore, TYafter,TXbefore, TXafter;
	float x01, x20, y01, y20;
	int ii,jj;
	if((b==0 && in.time_sense=='-')|| (b==0 && in.time_sense=='+')) // falling
	{
		int k=-1; int z=-1;
		for(int i=0; i<in.fallCLK_index1.size(); i++)
			if(in.fallCLK_index1[i]==index1)
			{k=i;break;}
			if(k==-1)
			{
				for(int i=0; i<in.fallCLK_index1.size(); i++)
					if(index1>in.fallCLK_index1[i])
					{
						Ybefore=in.fallCLK_index1[i]; ii=i;
						if(i<in.fallCLK_index1.size()-1)
							Yafter=in.fallCLK_index1[i+1];
						else if(i==in.fallCLK_index1.size()-1)
						{Ybefore=in.fallCLK_index1[i-1]; ii=i;Yafter=in.fallCLK_index1[i]; }
					}
			}
		for(int i=0; i<in.fallCLK_index2.size(); i++)
			if(in.fallCLK_index2[i]==index2)
			{z=i;break;}
			if(z==-1)
			{
				for(int i=0; i<in.fallCLK_index2.size(); i++)
					if(index2>in.fallCLK_index2[i])
					{
						Xbefore=in.fallCLK_index2[i]; jj=i;
						if(i<in.fallCLK_index2.size()-1)
							Xafter=in.fallCLK_index2[i+1];
						else if(i==in.fallCLK_index2.size()-1)
						{Xbefore=in.fallCLK_index2[i-1]; jj=i; Xafter=in.fallCLK_index2[i]; }
					}
			}
			if(k==-1 && z==-1)
			{
				x01=index2-Xbefore / Xafter-Xbefore;
				x20=Xafter-index2 / Xafter-Xbefore;
				y01=index1-Ybefore / Yafter-Ybefore;
				y20=Yafter-index1 / Yafter-Ybefore;
				value= x20*y20*in.fallCLK[ii][jj]+ x20*y01*in.fallCLK[ii][jj+1]+ x01*y20*in.fallCLK[ii+1][jj]+ x20*y20*in.fallCLK[ii+1][jj+1];
			}
			else
				value=in.fallCLK[k][z];
	}
	//if((b==1 && in.time_sense=='+') ||(b==1 && in.time_sense=='-')  )// rising
	else
	{
		int k=-1; int z=-1;
		for(int i=0; i<in.riseCLK_index1.size(); i++)
			if(in.riseCLK_index1[i]==index1)
			{k=i;break;}
			if(k==-1)
			{
				for(int i=0; i<in.riseCLK_index1.size(); i++)
					if(index1>in.riseCLK_index1[i])
					{
						Ybefore=in.riseCLK_index1[i]; ii=i;
						if(i<in.riseCLK_index1.size()-1)
							Yafter=in.riseCLK_index1[i+1];
						else if(i==in.riseCLK_index1.size()-1)
						{Ybefore=in.riseCLK_index1[i-1]; ii=i;Yafter=in.riseCLK_index1[i]; }
					}
			}
		for(int i=0; i<in.riseCLK_index2.size(); i++)
			if(in.riseCLK_index2[i]==index2)
			{z=i;break;}
			if(z==-1)
			{
				for(int i=0; i<in.riseCLK_index2.size(); i++)
					if(index2>in.riseCLK_index2[i])
					{
						Xbefore=in.riseCLK_index2[i]; jj=i;
						if(i<in.riseCLK_index2.size()-1)
							Xafter=in.riseCLK_index2[i+1];
						else if(i==in.riseCLK_index2.size()-1)
						{Xbefore=in.riseCLK_index2[i-1]; jj=i; Xafter=in.riseCLK_index2[i]; }
					}
			}
			if(k==-1 && z==-1)
			{
				x01=index2-Xbefore / Xafter-Xbefore;
				x20=Xafter-index2 / Xafter-Xbefore;
				y01=index1-Ybefore / Yafter-Ybefore;
				y20=Yafter-index1 / Yafter-Ybefore;
				value= x20*y20*in.riseCLK[ii][jj]+ x20*y01*in.riseCLK[ii][jj+1]+ x01*y20*in.riseCLK[ii+1][jj]+ x20*y20*in.riseCLK[ii+1][jj+1];
			}
			else
				value=in.riseCLK[k][z];
	}
	
	return value;
}
float Parser::getCellDelay(string celltype, string pin, float outputcap, float inputtrans)
{
	float temp = max(Cell_Delay(celltype, pin, outputcap, inputtrans,0), Cell_Delay(celltype, pin, outputcap, inputtrans, 1));
	return temp;
}
float Parser::getCellTransition(string celltype, string pin, float outputcap,float inputtrans)
{
	float temp = max(Cell_Transition(celltype, pin, outputcap, inputtrans, 0), Cell_Transition(celltype, pin, outputcap, inputtrans, 1));
	return temp;
}
float Parser::input_capac(string cell, string pin)
{
	PinI in=Return_Pin(cell,pin);
	return in.capacitance;
}
///////////////////////////////////////////////////////////////////////////////
int   Parser::char_int(char c)
{
	int i;
	if(c=='1')
		return 1;
	if(c=='2')
		return 2;
	if(c=='3')
		return 3;
	if(c=='4')
		return 4;
	if(c=='5')
		return 5;
	if(c=='6')
		return 6;
	if(c=='7')
		return 7;
	if(c=='8')
		return 8;
	if(c=='9')
		return 9;
	if(c=='10')
		return 10;
}
float Parser::Cell_Delay(string cell, string pin, float index1, float index2, int b)
{
	float value;
	PinI in=Return_Pin(cell,pin);
	float Ybefore, Yafter,Xbefore, Xafter;
	float TYbefore, TYafter,TXbefore, TXafter;
	float x01, x20, y01, y20;
	int ii,jj;
	if((b==0 && in.time_sense=='-')|| (b==0 && in.time_sense=='+')) // falling
	{
		int k=-1; int z=-1;
		for(int i=0; i<in.fallC_index1.size(); i++)
			if(in.fallC_index1[i]==index1)
			{k=i;break;}
			if(k==-1)
			{
				for(int i=0; i<in.fallC_index1.size(); i++)
					if(index1>in.fallC_index1[i])
					{
						Ybefore=in.fallC_index1[i]; ii=i;
						if(i<in.fallC_index1.size()-1)
							Yafter=in.fallC_index1[i+1];
						else if(i==in.fallC_index1.size()-1)
						{Ybefore=in.fallC_index1[i-1]; ii=i;Yafter=in.fallC_index1[i]; }
					}
			}
		for(int i=0; i<in.fallC_index2.size(); i++)
			if(in.fallC_index2[i]==index2)
			{z=i;break;}
			if(z==-1)
			{
				for(int i=0; i<in.fallC_index2.size(); i++)
					if(index2>in.fallC_index2[i])
					{
						Xbefore=in.fallC_index2[i]; jj=i;
						if(i<in.fallC_index2.size()-1)
							Xafter=in.fallC_index2[i+1];
						else if(i==in.fallC_index2.size()-1)
						{Xbefore=in.fallC_index2[i-1]; jj=i; Xafter=in.fallC_index2[i]; }
					}
			}
			if(k==-1 && z==-1)
			{
				x01=index2-Xbefore / Xafter-Xbefore;
				x20=Xafter-index2 / Xafter-Xbefore;
				y01=index1-Ybefore / Yafter-Ybefore;
				y20=Yafter-index1 / Yafter-Ybefore;
				value= x20*y20*in.fallC[ii][jj]+ x20*y01*in.fallC[ii][jj+1]+ x01*y20*in.fallC[ii+1][jj]+ x20*y20*in.fallC[ii+1][jj+1];
			}
			else
				value=in.fallC[k][z];
	}
	//if((b==1 && in.time_sense=='+') ||(b==1 && in.time_sense=='-')  )// rising
	else
	{
		int k=-1; int z=-1;
		for(int i=0; i<in.riseC_index1.size(); i++)
			if(in.riseC_index1[i]==index1)
			{k=i;break;}
			if(k==-1)
			{
				for(int i=0; i<in.riseC_index1.size(); i++)
					if(index1>in.riseC_index1[i])
					{
						Ybefore=in.riseC_index1[i]; ii=i;
						if(i<in.riseC_index1.size()-1)
							Yafter=in.riseC_index1[i+1];
						else if(i==in.riseC_index1.size()-1)
						{Ybefore=in.riseC_index1[i-1]; ii=i;Yafter=in.riseC_index1[i]; }
					}
			}
		for(int i=0; i<in.riseC_index2.size(); i++)
			if(in.riseC_index2[i]==index2)
			{z=i;break;}
			if(z==-1)
			{
				for(int i=0; i<in.riseC_index2.size(); i++)
					if(index2>in.riseC_index2[i])
					{
						Xbefore=in.riseC_index2[i]; jj=i;
						if(i<in.riseC_index2.size()-1)
							Xafter=in.riseC_index2[i+1];
						else if(i==in.riseC_index2.size()-1)
						{Xbefore=in.riseC_index2[i-1]; jj=i; Xafter=in.riseC_index2[i]; }
					}
			}
			if(k==-1 && z==-1)
			{
				x01=index2-Xbefore / Xafter-Xbefore;
				x20=Xafter-index2 / Xafter-Xbefore;
				y01=index1-Ybefore / Yafter-Ybefore;
				y20=Yafter-index1 / Yafter-Ybefore;
				value= x20*y20*in.riseC[ii][jj]+ x20*y01*in.riseC[ii][jj+1]+ x01*y20*in.riseC[ii+1][jj]+ x20*y20*in.riseC[ii+1][jj+1];
			}
			else
				value=in.riseC[k][z];
	}
	
	return value;
}
float Parser:: Cell_Transition(string cell, string pin, float index1, float index2, int b)
{
	float value;
	PinI in=Return_Pin(cell,pin);
	float Ybefore, Yafter,Xbefore, Xafter;
	float TYbefore, TYafter,TXbefore, TXafter;
	float x01, x20, y01, y20;
	int ii,jj;
	if((b==0 && in.time_sense=='-')|| (b==0 && in.time_sense=='+')) // falling
	{
		int k=-1; int z=-1;
		for(int i=0; i<in.fallT_index1.size(); i++)
			if(in.fallT_index1[i]==index1)
			{k=i;break;}
			if(k==-1)
			{
				for(int i=0; i<in.fallT_index1.size(); i++)
					if(index1>in.fallT_index1[i])
					{
						Ybefore=in.fallT_index1[i]; ii=i;
						if(i<in.fallT_index1.size()-1)
							Yafter=in.fallT_index1[i+1];
						else if(i==in.fallT_index1.size()-1)
						{Ybefore=in.fallT_index1[i-1]; ii=i;Yafter=in.fallT_index1[i]; }
					}
			}
		for(int i=0; i<in.fallT_index2.size(); i++)
			if(in.fallT_index2[i]==index2)
			{z=i;break;}
			if(z==-1)
			{
				for(int i=0; i<in.fallT_index2.size(); i++)
					if(index2>in.fallT_index2[i])
					{
						Xbefore=in.fallT_index2[i]; jj=i;
						if(i<in.fallT_index2.size()-1)
							Xafter=in.fallT_index2[i+1];
						else if(i==in.fallT_index2.size()-1)
						{Xbefore=in.fallT_index2[i-1]; jj=i; Xafter=in.fallT_index2[i]; }
					}
			}
			if(k==-1 && z==-1)
			{
				x01=index2-Xbefore / Xafter-Xbefore;
				x20=Xafter-index2 / Xafter-Xbefore;
				y01=index1-Ybefore / Yafter-Ybefore;
				y20=Yafter-index1 / Yafter-Ybefore;
				value= x20*y20*in.fallT[ii][jj]+ x20*y01*in.fallT[ii][jj+1]+ x01*y20*in.fallT[ii+1][jj]+ x20*y20*in.fallT[ii+1][jj+1];
			}
			else
				value=in.fallT[k][z];
	}
	//if((b==1 && in.time_sense=='+') ||(b==1 && in.time_sense=='-')  )// rising
	else
	{
		int k=-1; int z=-1;
		for(int i=0; i<in.riseT_index1.size(); i++)
			if(in.riseT_index1[i]==index1)
			{k=i;break;}
			if(k==-1)
			{
				for(int i=0; i<in.riseT_index1.size(); i++)
					if(index1>in.riseT_index1[i])
					{
						Ybefore=in.riseT_index1[i]; ii=i;
						if(i<in.riseT_index1.size()-1)
							Yafter=in.riseT_index1[i+1];
						else if(i==in.riseT_index1.size()-1)
						{Ybefore=in.riseT_index1[i-1]; ii=i;Yafter=in.riseT_index1[i]; }
					}
			}
		for(int i=0; i<in.riseT_index2.size(); i++)
			if(in.riseT_index2[i]==index2)
			{z=i;break;}
			if(z==-1)
			{
				for(int i=0; i<in.riseT_index2.size(); i++)
					if(index2>in.riseT_index2[i])
					{
						Xbefore=in.riseT_index2[i]; jj=i;
						if(i<in.riseT_index2.size()-1)
							Xafter=in.riseT_index2[i+1];
						else if(i==in.riseT_index2.size()-1)
						{Xbefore=in.riseT_index2[i-1]; jj=i; Xafter=in.riseT_index2[i]; }
					}
			}
			if(k==-1 && z==-1)
			{
				x01=index2-Xbefore / Xafter-Xbefore;
				x20=Xafter-index2 / Xafter-Xbefore;
				y01=index1-Ybefore / Yafter-Ybefore;
				y20=Yafter-index1 / Yafter-Ybefore;
				value= x20*y20*in.riseT[ii][jj]+ x20*y01*in.riseT[ii][jj+1]+ x01*y20*in.riseT[ii+1][jj]+ x20*y20*in.riseT[ii+1][jj+1];
			}
			else
				value=in.riseT[k][z];
	}
	
	return value;
}
Cell Parser::Return_Cell(string name)
{
	for(int i=0; i<library.size();i++)
		if(library[i].nameC==name)
			return library[i];
	cout<<"This Cell Does not exist in the library!!"<<endl;
}
PinI Parser::Return_Pin(string cell,string name)
{
	Cell k= Return_Cell(cell);
	for(int i=0; i<k.pins.size();i++)
		if(k.pins[i].nameP==name)
			return k.pins[i];
}

