#include<iostream>
#include<string.h>
#include <cstdlib>
#include <ctime>
#include<math.h>
#include<windows.h>
using namespace std;
char correctCW[200];
char single_bit_error[200];
char double_bit_error[200];
int r;
class sender
{
public:
    int codeword;
    int msg_len;

public:
    int get_codeword_size(char massage[30])
    {
        r=0;

        msg_len=strlen(massage);
        while(1)
        {
            if(pow(2,r)>=(msg_len+r+1))
                break;

            r++;
        }
        codeword = msg_len+r;
        return codeword;
    }

    void set_parity_position(char massage[20])
    {
        int j=0,k;
        k = msg_len - 1;

        for(int i=1; i<=codeword; i++)
        {
            int p = pow(2,j);
            if(i%p == 0)
            {
                correctCW[i]='0';
                j++;
            }
            else
            {
                correctCW[i] = massage[k];
                k--;

            }
        }
    }

    void generate_codeword(int parity)
    {
        int i,k,j,count,p;
        for(i=0; i<=r-1; i++)
        {
            count = 0;
            p = pow(2,i);
            k = 0;
            for(j=0; j<=codeword; j++)
            {

                if(k==1)
                {
                    if(correctCW[j]=='1')
                        count++;

                    if((j+1)%p==0)
                        k=0;
                }
                else if(k == 0)
                {
                    if((j+1)%p == 0)
                        k = 1;
                }

            }

            if(parity == 0 && count%2 != 0)
            {
                correctCW[p] = '1';

            }

            else if(parity == 1 && count%2 == 0)
            {
                correctCW[p]='1';
            }
        }
        cout<<"\n\n\tGenerated codeword       : ";
        for(i=codeword; i>=1; i--)
        {
            cout<<correctCW[i];
        }

    }


};

class receiver
{
public:
    int pos[10];
    int error_index;

public:
    void receive(char massage[20],int n,int parity)
    {
        int i,shift,j,count,p,index=0;
        for(i=0; i<=r-1; i++)
        {
            count=0;
            p = pow(2,i);
            shift=0;
            for(j=0; j<=n; j++)
            {

                if(shift==1)
                {
                    if(massage[j]=='1')
                        count++;

                    if((j+1)%p==0)
                        shift=0;
                }
                else if(shift==0)
                {
                    if((j+1)%p==0)
                        shift=1;
                }




            }


            if(parity==0)
            {
                if(count%2!=0)
                {

                    pos[index]=1;
                    index++;

                }
                else
                {
                    pos[index]=0;
                    index++;
                }
            }

            if(parity==1)
            {
                if(count%2==0)
                {
                    pos[index]=1;
                    index++;

                }

                else
                {
                    pos[index]=0;
                    index++;
                }
            }


        }

        for(i=0; i<r; i++)
        {
            error_index=error_index+(pos[i]*pow(2,i));

        }

        if(error_index==0)
        {
            cout<<"\nNo error detected in codeword!";
        }

        else
        {
            cout<<"\nError detected in codeword on "<< error_index<<"th index!";
        }
    }

};

void single_error(int n)
{
    int i,random_index;
    for(i=1; i<=n; i++)
    {
        single_bit_error[i]=correctCW[i];
    }
    srand(time(0));

    random_index = (rand()%n)+1;

    if(single_bit_error[random_index]=='0')
    {
        single_bit_error[random_index]='1';

    }
    else
    {
        single_bit_error[random_index]='0';
    }

    cout<<"\n\tSingle Bit error codeword: ";
    for(i=n; i>=1; i--)
    {
        cout <<single_bit_error[i];
    }


}


void double_error(int n)
{
    int i,rand_idx,rand_idx_che;
    for(i=1; i<=n; i++)
    {
        double_bit_error[i]=correctCW[i];
    }
    srand((unsigned)time(0));

    rand_idx = (rand()%n)+1;

    if(double_bit_error[rand_idx]=='0')
    {
        double_bit_error[rand_idx]='1';

    }

    else
    {
        double_bit_error[rand_idx]='0';
    }

    while(1)
    {
        rand_idx_che = (rand()%n)+1;
        if(rand_idx_che!=rand_idx)
        {
            if(double_bit_error[rand_idx_che]=='0')
            {
                double_bit_error[rand_idx_che]='1';

            }

            else
            {
                double_bit_error[rand_idx_che]='0';
            }

            break;
        }

    }

    cout<<"\n\tDouble Bit error codeword: ";
    for(i=n; i>=1; i--)
    {
        cout<<double_bit_error[i];
    }


}


int main()
{
    char data[30];
    int word_size;
    int p_t;
    cout<<"\n\t\t\t\t\tHamming Code";
    cout<<"\n\nBinary Massage:";
    cin>>data;

    cout<<"\nParity type:\n\t1.For even press 0\n\t2.For odd press 1:";
    cin>>p_t;
    sender s_obj;
    receiver r_obj;
    word_size=s_obj.get_codeword_size(data);
    s_obj.set_parity_position(data);
    s_obj.generate_codeword(p_t);
    single_error(word_size);
    double_error(word_size);

    cout << "\n\n\n";
    cout<<"\nReceiving......";
    cout<<"\n\nAfter sending correct codeword at receiver:";
    r_obj.receive(correctCW,word_size,p_t);
    cout<<"\n\nAfter sending single bit error at receiver:";
    r_obj.receive(single_bit_error,word_size,p_t);
    cout<<"\n\nAfter sending double bit at receiver:";
    r_obj.receive(double_bit_error,word_size,p_t);
    cout<<endl;

    return 0;
}
