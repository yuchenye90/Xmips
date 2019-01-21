#include "component.h"

int assembler::trans(char *op, int line){	//translate aux to digit
	if(!strcmp(op, "MOV") || !strcmp(op, "mov"))     return 900152;
	//if(!strcmp(op, "IMMOV") || !strcmp(op, "immov")) return 90112;	//IMMOV, M, and ASN image to same code
	//if(!strcmp(op, "M") || !strcmp(op, "m"))         return 90112;
	//if(!strcmp(op, "ASN") || !strcmp(op, "asn"))     return 90112; 
	if(!strcmp(op, "ADD") || !strcmp(op, "add"))     return 900272;
	if(!strcmp(op, "SUB") || !strcmp(op, "sub"))     return 901072;
	if(!strcmp(op, "MUL") || !strcmp(op, "mul"))     return 902072;
	if(!strcmp(op, "DIV") || !strcmp(op, "div"))     return 903072;
	if(!strcmp(op, "MOD") || !strcmp(op, "mod"))     return 904072;
	if(!strcmp(op, "AND") || !strcmp(op, "and"))     return 905072;
	if(!strcmp(op, "OR") || !strcmp(op, "or"))       return 906072;
	if(!strcmp(op, "XOR") || !strcmp(op, "xor"))     return 907072;
	//if(!strcmp(op, "IMADD") || !strcmp(op, "imadd")) return 901272;
	//if(!strcmp(op, "A") || !strcmp(op, "a"))         return 901272;
	if(!strcmp(op, "CMP") || !strcmp(op, "cmp"))     return 900332;
	//if(!strcmp(op, "IMCMP") || !strcmp(op, "imcmp")) return 90132;
	//if(!strcmp(op, "C") || !strcmp(op, "c"))         return 90132;
	if(!strcmp(op, "JA") || !strcmp(op, "ja"))       return 900421;
	if(!strcmp(op, "JB") || !strcmp(op, "jb"))       return 900521;
	if(!strcmp(op, "JMP") || !strcmp(op, "jmp"))     return 900621;
	if(!strcmp(op, "JE") || !strcmp(op, "je"))       return 900721;
	if(!strcmp(op, "JNE") || !strcmp(op, "jne"))     return 900821;
	if(!strcmp(op, "INC") || !strcmp(op, "inc"))     return 900961;
	if(!strcmp(op, "NEG") || !strcmp(op, "neg"))     return 901961;
	if(!strcmp(op, "NOT") || !strcmp(op, "not"))     return 902961;
	//if(!strcmp(op, "IMSUB") || !strcmp(op, "imsub")) return 90142;
	//if(!strcmp(op, "S") || !strcmp(op, "s"))         return 90142;
	if(!strcmp(op, "LEA") || !strcmp(op, "lea"))     return 902042;	//has better way now
	if(!strcmp(op, "PUSH") || !strcmp(op, "push"))   return 902121;
	if(!strcmp(op, "POP") || !strcmp(op, "pop"))     return 902241;
	if(!strcmp(op, "PUSHA") || !strcmp(op, "pusha")) return 902300;
	if(!strcmp(op, "POPA") || !strcmp(op, "popa"))   return 902400;
	//if(!strcmp(op, "OFST") || !strcmp(op, "ofst"))   return 90302;
	if(!strcmp(op, "LOC") || !strcmp(op, "loc"))     return 910002;
	if(!strcmp(op, "DIM") || !strcmp(op, "dim"))     return 910002; //dim is newer than loc
	if(!strcmp(op, "END") || !strcmp(op, "end"))     return 900000;
	if(!strcmp(op, "INT") || !strcmp(op, "int"))	 return 904000;
	if(!strcmp(op, "CALL") || !strcmp(op, "call"))   return 904121;
	if(!strcmp(op, "RET") || !strcmp(op, "ret"))	 return 904200;
	if(!strcmp(op, "IRET") || !strcmp(op, "iret"))   return 904300;
	if(!strcmp(op, "SYSR") || !strcmp(op, "sysr"))   return 950000;
	if(!strcmp(op, "SYSW") || !strcmp(op, "sysw"))   return 950100;
	if(!strcmp(op, "WAKE") || !strcmp(op, "wake"))   return 950200;
	if(!strcmp(op, "SET") || !strcmp(op, "set"))     return 950300;
	if(!strcmp(op, "CLI") || !strcmp(op, "cli"))     return 960000;
	if(!strcmp(op, "STI") || !strcmp(op, "sti"))     return 960100;
    if(!strcmp(op, "HALT") || !strcmp(op, "halt"))   return 800000;
	if(!strcmp(op, "~"))     return 3; //comment
	if(!strcmp(op, "CMNT") || !strcmp(op, "cmnt"))   return 3; //also comment
	if(!strcmp(op, "$"))     return 0; //filler
	if(!strcmp(op, ":"))	return 920001; //������
	if(!strcmp(op, "PROC") || !strcmp(op, "proc"))   return 920001; //use tag

	//RES=50;
	//if(systemChecker.showLevel(RES, sysLog)){
	//	printf("D%-5d row:%d, opcode:\'%s\' ",ID,line,op);
	//	systemChecker.check(RES, sysLog);
	//}
	return -1;
}



//��ʽ��ַ������
//&��ʾƫ��Ѱַ��ֻ������ڼĴ���1~9����ȨֵΪ8�������ϼĴ����ų���100������&3����Ӧ��ֵΪ8+3*100=308
//@��ʾ���Ѱַ��ȨֵΪ4
//#��ʾ�Ĵ���Ѱַ��ȨֵΪ2
//����ʾ��������ȨֵΪ3
//��һ��������addressing����Ѱַ��ʽ
//����Ѱַ��ʽ����ͬʱ���ڣ�addressingΪ����Ѱַ��ʽȨֵ�ĺ�
//���� &4@#2������ƫ��Ѱַ�����Ѱַ�ͼĴ���Ѱַ��EA=(R4)+(R2) addressing=8+4*100+4+2=414
//&4#2 ����Ч�ģ���Ϊ�Ĵ���2ֻ�з��ŵ�ַ��û��ʵ�ʵ�ַ!
//&5@100 EA=(R5)+(100)
//@!32 ����Ч�ģ�����������ֱ��ʹ��!����!5��
int assembler::trans2(char *op, varNote *vp, int line,int &addressing){	//ptrflag = 1 means op is ptr var
	int i;
	char *ptmp=op;	//ָ���������ַ��ָ��
	int advisit=0;	//���ڼ���Ѱַ��ʽ
	int ptrflag; //�Ƿ�����˼��Ѱַ��ʽ

	addressing=0;

	if(!strcmp(op,"$")){
		addressing=0;	
		return 0;	//ռλ���������Ѿ����Բ���
	}

	if(*ptmp=='&'){	//ƫ��Ѱַ
		advisit+=8;	//
		if(*(ptmp+1)>='1' && *(ptmp+1)<='9'){	//ֻ����ԼĴ���1~9
			advisit+=100*(*(ptmp+1)-'0');	//���ϼĴ����ų���100
			ptmp+=2;		//ָ��������ַ����е���һ���ַ�
		}
		else{	//���ʽ���󣬴����ʹ���˷���&
			RES=53;
			if(systemChecker.showLevel(RES, sysLog)){
				printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
				systemChecker.check(RES, sysLog);
			}
			return -1;
		}
	}

	if(*ptmp=='@'){	//���Ѱַ
		advisit+=4;
		ptmp++;
		ptrflag=1;	//��ʾ�����˼��Ѱַ��ʽ��
	}
	
	if(*ptmp=='#' || *ptmp=='!'){
		//advisit+=2;!!!
		if(*ptmp=='#'){	//�Ĵ���Ѱַ
			if(advisit % 10==8){	//��������&4#2����ʽ������ʽ����Ч�ģ���Ϊ�Ĵ���2ֻ�з��ŵ�ַ��û��ʵ�ʵ�ַ! 
				RES=54;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
					systemChecker.check(RES, sysLog);
				}
				return -1;	
			}
			if( *(ptmp+1)>='0' && *(ptmp+1)<='9' ){	//'#'���ź����Ϊһ������Ĵ����ŵ�����
				int registerNO;
				advisit+=2;
				addressing=advisit;
				
				registerNO=atoi(ptmp+1);
				if(registerNO>14){
					RES=58;
					if(systemChecker.showLevel(RES, sysLog)){
						printf("D%-5d row:%d, register No:%d ",ID,line,registerNO);
						systemChecker.check(RES, sysLog);
					}
					return -1;
				}
				else
					return registerNO;
			}
			else{	//�Ĵ���Ѱַ���ʽ����
				RES=55;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
					systemChecker.check(RES, sysLog);
				}
				return -1;
			}
		}
		else{	//��Ϊ����Ѱַ!!! //������
			/*
			if((advisit-2)!=0){	//������֮ǰ��������������
				RES=56;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
					systemChecker.check(RES, sysLog);
				}
				return -1;	
			}
			*/

			//advisit++;

			//if(*(ptmp+1)=='-' || *(ptmp+1)=='+'|| (*(ptmp+1)>='0' && *(ptmp+1)<='9')){	//�������ĵ�һ���ַ����������ֻ�������
			//	addressing=advisit;
			//	return atoi(ptmp+1);
			//}
			if((*(ptmp+1)>='0' && *(ptmp+1)<='9')){	//�������ĵ�һ���ַ����������ֻ�������
				addressing=advisit;
				return atoi(ptmp+1);
			}


			else{	//'!'����֮��ĸ�ʽ����
				RES=57;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
					systemChecker.check(RES, sysLog);
				}
				return -1;
			}
		}
	}

	else{	//��Ϊ��ʽ��ַΪ������ŵ�ַ����������	//��ʽ��ַ���������ַ���������ŵ�ַ��
		i=0;
		while(vp[i].valid==1){	//���ҷ��ŵ�ַ��
			if(!strcmp(ptmp,vp[i].var)){	//�ñ�ʾ�����Ѿ�����ķ��ŵ�ַ	
				addressing=advisit;
				return vp[i].pos;
			}
			i++;
		}

		if(ptrflag==1 ){	//�Ѿ�ʹ����'@'���ţ��������ı�ʶ�������Ѿ�����ķ��ŵ�ַ
			if(*ptmp<'0' || *ptmp>'9'){	//δ�����ʶ���������֣�����
				RES=51;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
					systemChecker.check(RES, sysLog);
				}
				return -1;
			}
			else{	//���޸ġ�@7�ǷǷ��ģ���Ϊ7����������ǰ�治�ɳ����������� //δ�����ʶ�������֣���ֱ�Ӳ����˼��Ѱַ�ķ�ʽ����@7 EA=��7��
				RES=56;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
					systemChecker.check(RES, sysLog);
				}
				return -1;			
			}
		}
		else{	//��Ϊ������Ϊ������ //���������������ַ
			if(*ptmp=='+' || *ptmp=='-' || (*ptmp>='0' && *ptmp<='9')){
				if(advisit !=0){
					RES=56;
					if(systemChecker.showLevel(RES, sysLog)){
						printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
						systemChecker.check(RES, sysLog);
					}
					return -1;
				}
				advisit+=3;
				addressing=advisit;
				return atoi(ptmp);	
			}

			RES=52; //������δ����ķ��ţ����ʽ�Ƿ�
			if(systemChecker.showLevel(RES, sysLog)){
				printf("D%-5d row:%d, operand:\'%s\' ",ID,line,op);
				systemChecker.check(RES, sysLog);
			}
			return -1; 
		}
	}

}

int editor::editorFromFile(FILE *pfile, assembler &A, int &dataNumInMData){	//editor in line ,dataNumInMData is num of data in MData
	int o,d,s;	//opcode, destination operand and source oprand
	//char line[5];
	char op[20];
	char od[20];
	char os[20];
	char comment[80]; //ע��
	int opcodeType=0; //with 0,1 or 2 operand
	int addressing=0; //address visit way
	int addressingUnion;	//used to handle double operand addressing
	int i=0,j=0;	//i ��¼ʵ���кţ�j ��¼��ʾ�к�
	int dataCounter=0;	//data dim counter. ��¼���������ݶ��е�λ�ã���0��ʼ��
	int state=0; //��¼��������0λ����������Ϊ��������

	int v=0;	//the var num
	int tagv=0;	//��������
	int jmpv=0;	//��ת�����

	varNote varTable[20];	//max numbers of variables :20
	varNote tagTable[20];	//tag table
	varNote2 jumpTable[20];	//table about jump instructions

	if(displayMode) printf("display your statememts\n");
	if(displayMode) printf("%-4d",j);

	//fscanf(pfile,"%s%s",line,op);	//��ȡ������
	fscanf(pfile,"%s",op);	//��ȡ�����룬ȡ���к�
	o=A.trans(op,j);  //need to change; trans char to int opcode
	opcodeType=o%10; //������������ע�͵�opcodeType=3

	switch(opcodeType){
		case 0: d=s=0;
			if(displayMode) printf("%s\n", op); //�޲�����
			break;
		case 1: s=0;
			fscanf(pfile,"%s",od);	//������������ȡĿ�Ĳ�����
			if(displayMode) printf("%s %s\n",op,od);
			break;
		case 2: 
			if(o!=910002){	//����dim��䣬���⴦��
				fscanf(pfile,"%s%s",od,os);	//˫����������ȡĿ�Ĳ�������Դ������
				if(displayMode) printf("%s %s %s\n",op,od,os);	//���������һ�����з������ڷ�������������(�޸�)
			}
			else{
				fscanf(pfile,"%s%s",od,os);	//˫����������ȡĿ�Ĳ�������Դ������
				if(displayMode) printf("%s %s %s ",op,od,os);	//���������һ�����з������ڷ�������������
			}
			break;
//		case 3:
//			fscanf(pfile,"%s",comment);
//			if(displayMode) printf("%s %s\n",op,comment);	//comment, do nothing
//			break;
		default: ;
	}

	if(o!=910002 && o!=920001 && o!=3 && opcodeType != 0){//not loc statement and not comment. ����Ѱַ��ʽ
		if(opcodeType==1){
			
			if(o!=900421 && o!=900521 && o!=900621 && o!=900721 && o!=900821 && o!=904121){	//������ת����call�����ò�ͬ�ĳ����������ķ�ʽ
				d=A.trans2(od,varTable,j,addressing);	//trans char to int operand
				addressingUnion=addressing;
			}
			//else
			//	addressingUnion=3; //������ת��䣬����൱��������
		}
		if(opcodeType==2 ){	//˫�����������㸴��Ѱַ��ʽ
			d=A.trans2(od,varTable,j,addressing);	//trans char to int operand
			if(addressing==3){
				RES=197;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d operand:\'%s\' ",ID,od);
					systemChecker.check(RES, sysLog);
				}
				state=8;
			}
			addressingUnion=addressing;
			s=A.trans2(os,varTable,j,addressing);
			addressingUnion+=addressing*1000;
		}
	}


	while(o!=900000 && o!=-1 && i<codeBufferSize){	//900000:end, -1:syntax error
		if(o==920001){//���
			//putchar('\n');
			int m=0;
			while(tagTable[m].valid!=0){ //������
				if(!strcmp(od,tagTable[m].var)){ //����ظ�
					RES=193;
					if(systemChecker.showLevel(RES, sysLog)){
						printf("D%-5d repeated tag:%s ",ID,od);
						systemChecker.check(RES, sysLog);
					}
					state=1;
					break;
				}
				m++;
			}
			
			//�������Ϣ�����ű�
			strcpy(tagTable[tagv].var,od);	//note the var name
			tagTable[tagv].pos=i;	//note the var pos
			tagTable[tagv].valid=1;		//valid==1 means the position in varTable has been used
			tagv++;			

		}
		else
		if(o==900421 || o==900521 || o==900621 || o==900721 || o==900821 || o==904121){	//����ת��Ϣ������ת��
			strcpy(jumpTable[jmpv].var,od);
			jumpTable[jmpv].pos=i;
			jumpTable[jmpv].valid=1;
			jmpv++;
			
			//putchar('\n');
			codeBuffer[i]=o;	//write to codeBuffer
			codeBuffer[i+1]=3;	//visit address way, immediate 3
			codeBuffer[i+3]=0;	//û��Դ������
			//pptr->MCode.write(i,o);	//write to MCode
			//pptr->MCode.write(i+1,3);	//visit address way, immediate 3
			//pptr->MCode.write(i+2,d);
			//pptr->MCode.write(i+3,s);	
			i+=4;
			j++;	//LOC statement not increase the num of line
		}
		else
		if(o==910002){ //note new var
			if((od[0] >='a' && od[0] <='z') || (od[0] >='A' && od[0] <='Z')){	//var name must begin with letter
			
				int t=0;
				while(varTable[t].valid!=0){
					if(!strcmp(od,varTable[t].var)){ //�����ظ�
						RES=196;
						if(systemChecker.showLevel(RES, sysLog)){
							printf("D%-5d repeated indentifier:%s ",ID,od);
							systemChecker.check(RES, sysLog);
						}
						state=2;
						break;
					}
					t++;
				}

				strcpy(varTable[v].var,od);	//note the var name
				//varTable[v].pos=atoi(os);	//note the var pos
				varTable[v].pos=dataCounter;	//note the var pos
				varTable[v].valid=1;		//valid==1 means the position in varTable has been used
				v++;
				
				
				int len;	//����������ĳ���
				int dataNum;	//����ֵ������Ԫ�صĸ���+��ǰ���ݶ��е�ƫ����
				char dataValue[40];	//��ֵ�ַ���
				char fc;	//���ڶ�ȡ�ļ��г�ֵ���ַ�
				int i=0;
				char *cp;	//��ֵ�ַ���ָ��
				int cflag=0;	//��ʾһ����ֵ�Ƿ�������Զ��ŷָ�
				int k;	//����ֵ�ַ����еĳ�ֵת��������
				
				if(os[0]=='D'){	//��ʾ�����������
					len=atoi(os+1);
					dataNum=dataCounter;
					dataCounter+=len;
				

					fc=fgetc(pfile);

					while(fc!=';'){	//��ֵ�ַ����ԷֺŽ���
						dataValue[i]=fc;	//�����ļ��ж�ȡ���ַ����浽����
						fc=fgetc(pfile);	//����һ���ַ�
						i++;
					}
					dataValue[i]=';';	//���ַ���ĩβ���Ϸֺţ���ʾ��ֵ����
					dataValue[i+1]='\0';	//��ʾ�ַ�������
					
					cp=dataValue;	//��cpָ���ֵ�ַ����ĵ�һ���ַ�

					while(*cp!=';'){//��ֵ�ַ����ԷֺŽ���
						if(((*cp>='0' && *cp <='9' ) || *cp=='-' || *cp=='+' || *cp=='\?' ) && cflag==0 ){
							if(dataNum+1>dataCounter){	//�Ѿ���ȡ�ĳ�ֵ������������ʱ��������鳤�ȣ�����
								putchar('\n');
								RES=191;
								if(systemChecker.showLevel(RES, sysLog)){
									printf("D%-5d ",ID);
									systemChecker.check(RES, sysLog);
								}
								state=3;
								break;
							}	
							k=atoi(cp);	//����ֵ�ַ����еĳ�ֵת��������
							//pptr->MData.write(dataNum,k); //����ֵ���浽���ݶ�
							dataBuffer[dataNum]=k;
							dataNum++;	//��ȡ���ַ�����1
							//printf("%d",k);
							cflag=1;
						}
						else
							if(*cp==','){	//�����˷ָ��ַ�����
								cflag=0;	//��ʾһ����ֵ����
							}

						cp++;	//ָ���ֵ�ַ����е���һ���ַ�
					}
					
					printf("%s\n",dataValue);
					dataNum=0;	//ȫ��������ȡ�ĳ�ֵ����0
				}
				else{
					putchar('\n');
					if((os[0]>='0' && os[0] <='9' ) || os[0]=='-' || os[0]=='+' || os[0]=='\?'){	//��������һ���ַ���ַ��һ��������,�ʺ�Ϊ��ֵ��ȷ��
						int k;
						k=atoi(os);	//��ȡ������ֵ
						//pptr->MData.write(dataCounter,k); //������д�����ݶ�
						dataBuffer[dataCounter]=k;
						dataCounter++;
					}
					else{	//��ʽ�Ƿ�
						RES=192;
						if(systemChecker.showLevel(RES, sysLog)){
							printf("D%-5d ",ID);
							systemChecker.check(RES, sysLog);
						}
						state=4;
					}
				}
	

			}
			else{	//�Ƿ����ŵ�ַ
				//putchar('\n');
				RES=13;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d variable name:\"%s\" ",ID,od);	//var name must begin with letter
					systemChecker.check(RES, sysLog);
				}
				state=5;
			}
		}//if(o==910002) end

		else{	//loc statement with line num 0
			
			if(o==3){
				fscanf(pfile,"%s",comment);
				if(displayMode) printf("%s %s\n",op,comment);	//comment, do nothing
			}
			else{
				//putchar('\n');
				//pptr->MCode.write(i,o);	//write to MCode
				
				if(o==800000) addressingUnion=d=s=0;	//����ͣ�����Ҫ���⴦������ͣ�������Ϊ�˷������Ƕ��ӳ���Ĵ���

				codeBuffer[i]=o;
				//pptr->MCode.write(i+1,addressingUnion);	//visit address way
				codeBuffer[i+1]=addressingUnion;
				//pptr->MCode.write(i+2,d);
				codeBuffer[i+2]=d;
				//pptr->MCode.write(i+3,s);
				codeBuffer[i+3]=s;

				i+=4;
				j++;	//LOC statement not increase the num of line
			}
		}

		if(displayMode) printf("%-4d",j);	//num of line
		
		fscanf(pfile,"%s",op);
		o=A.trans(op,j);  //need to change; trans char to int opcode
		opcodeType=o%10;

		switch(opcodeType){
			case 0: d=s=0;
				if(displayMode) printf("%s\n", op); //�޲�����
				break;
			case 1: s=0;
				fscanf(pfile,"%s",od);	//������������ȡĿ�Ĳ�����
				if(displayMode) printf("%s %s\n",op,od);
				break;
			case 2: 
				if(o!=910002){	//����dim��䣬���⴦��
					fscanf(pfile,"%s%s",od,os);	//˫����������ȡĿ�Ĳ�������Դ������
					if(displayMode) printf("%s %s %s\n",op,od,os);	//���������һ�����з������ڷ�������������(�޸�)
				}
				else{
					fscanf(pfile,"%s%s",od,os);	//˫����������ȡĿ�Ĳ�������Դ������
					if(displayMode) printf("%s %s %s ",op,od,os);	//���������һ�����з������ڷ�������������
				}
				break;
//			case 3:
//				fscanf(pfile,"%s",comment);
//				if(displayMode) printf("%s\n",comment);	//comment, do nothing
//				break;
			default: ;
		}

		if(o!=910002 && o!=920001 && o!=3 && opcodeType != 0){//not loc statement and not comment 
			
			if(opcodeType==1){
				if(o!=900421 && o!=900521 && o!=900621 && o!=900721 && o!=900821 && o!=904121){	//������ת����call�����ò�ͬ�ĳ����������ķ�ʽ
					d=A.trans2(od,varTable,j,addressing);	//trans char to int operand
					addressingUnion=addressing;
				}
			}

			if(opcodeType==2 ){	
				d=A.trans2(od,varTable,j,addressing);	//trans char to int operand
				if(addressing==3){
					RES=197;
					if(systemChecker.showLevel(RES, sysLog)){
						printf("D%-5d operand:\'%s\' ",ID,od);
						systemChecker.check(RES, sysLog);
					}
					state=8;
				}
				addressingUnion=addressing;
				s=A.trans2(os,varTable,j,addressing);
				addressingUnion+=addressing*1000;
			}
		}

			
		//fscanf(pfile,"%s%s%s%s",line,op,od,os);
		//if(displayMode) printf("%s %s %s",op,od,os);
		//getchar();

		//o=A.trans(op,j);

		//opcodeType=o%10;

		//if(o!=910002 && o!=1 && opcodeType!=0 ){
		//	d=A.trans2(od,varTable,j,addressing);
		//	addressingUnion=addressing;
		//	s=A.trans2(os,varTable,j,addressing);
		//	addressingUnion+=addressing*1000;

		//	printf("%d",addressingUnion);
		//}		
	}//while(o!=900000 && o!=-1 && i<pptr->MCode.mSize) end

	//putchar('\n');	//����һ�����з�

	if(o==900000 && state==0){	//normal end
		//�����޸���ת��ʶ
		int m=0;
		int n=0;
		while(tagTable[m].valid!=0){ //���ձ�ű����ת������תλ��д�����Σ� ����ĵ����������ת��
			n=0;
			while(jumpTable[n].valid!=0){
				if(jumpTable[n].found==0){
					if(!strcmp(jumpTable[n].var,tagTable[m].var)){
						//pptr->MCode.write(jumpTable[n].pos+2,tagTable[m].pos);
						codeBuffer[jumpTable[n].pos+2]=tagTable[m].pos;
						jumpTable[n].found=1;	//��ʾ����תλ��д��
					}	
				}
				n++;
			}
			m++;
		}

		m=0;
		while(jumpTable[m].valid!=0){	//�鿴ʱ����û�ж�Ӧ��ŵ���ת��
			if(jumpTable[m].found==0){
				RES=194;
				if(systemChecker.showLevel(RES, sysLog)){
					printf("D%-5d unfound tag:%s ",ID,jumpTable[m].var);
					systemChecker.check(RES, sysLog);
				}
				state=7;
				break;
			}
			m++;
		}

		if(displayMode) printf("end edit\n");
		//pptr->MCode.write(i,o);
		codeBuffer[i]=o;

		i++;
		dataNumInMData=dataCounter; //���ݶ������ݵĸ���
		return i;

	}
	else{	//state!=0, abend
		if(o==-1){	
			
			RES=14;
			if(systemChecker.showLevel(RES, sysLog)){
				printf("D%-5d opcode:\"%s\" ",ID,op);
				systemChecker.check(RES, sysLog);
			}
		}
		else
			if(state==0){	//����̫��,�����˴��뻺�����Ĵ�С			
				RES=15;
				if(systemChecker.showLevel(RES, sysLog)){
				printf("D%-5d numbers of lines:%d ",ID,i/4);
				systemChecker.check(RES, sysLog);
			}
		}
		return -1;
	}
}


int editor::ASM(FILE *pfile, assembler &A, char *s){
	int r;
	int dataInMData;	//���ݶ����ݵ���Ŀ
	r=editorFromFile(pfile,A,dataInMData);
	if(r==-1) return -1; //editor abend
	else{
		//saveToFile(pptr,r,dataInMData);
		FILE *fsave;		
		char tmps[50]; //���˱����µ��ļ���
		char *p;
		char ext[10]=".co"; //�ַ����ļ�����չ��Ϊco

		strcpy(tmps,s);
		p=tmps;

		while(*p!='.') p++;	//���ַ����ļ�����չ����Ϊxo
		*p='\0';
		strcat(tmps,ext);
		
		if((fsave=fopen(tmps, "w"))==0){	//�ļ���ʧ��
			RES=195;
			if(systemChecker.showLevel(RES, sysLog)){
				printf("D%-5d ",ID);
				systemChecker.check(RES, sysLog);
			}		
		}

		//saveMem(fsave, r);
		//saveMem(fsave, dataInMData);
		
		int i;
		for(i=0; i<r; i++){
			fprintf(fsave,"%d ",codeBuffer[i]);
		}

		fprintf(fsave,"%d ",dataInMData); //�����ݳ���д���ļ��������д

		for(i=0; i<dataInMData ; i++){
			fprintf(fsave,"%d ",dataBuffer[i]);
		}
			
		fclose(fsave);
		return 0;
	}		
}

int editor::editorDataFromFile(FILE *pfile, memory &M, assembler &A){	//edit the content of memory at one time
	char line[5]; //row number
	char r[10];
	int i=0;

	if(displayMode) printf("enter the content in this memory ID:%d\n",M.ID);
	if(displayMode) printf("%-4d",i);	//num of line
	fscanf(pfile,"%s%s",line,r);
	if(displayMode) printf("%s\n", r);
	//getchar();
	while((r[0]=='-' || (r[0]>='0' && r[0]<='9')) && i<M.mSize){	//end edit
		M.write(i,atoi(r));	//trans r to num
		i++;
		if(displayMode) printf("%-4d",i);
		fscanf(pfile,"%s%s",line,r);
		if(displayMode) printf("%s\n", r);
		//getchar();
	}
	if(!strcmp(r,"END") || !strcmp(r,"end")){
		if(displayMode) printf("end edit\n");
		return 0;
	}
	else{
		
		RES=12;
		if(systemChecker.showLevel(RES, sysLog)){
			printf("D%-5d data:\"%s\" ",ID,r);
			systemChecker.check(RES, sysLog);
		}
		return -1;
	}
}