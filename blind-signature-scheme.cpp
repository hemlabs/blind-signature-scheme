#include <iostream>
#include <utility>
#include <string>
#include <cctype>

using namespace std;

class Participant {
public:
    string name;
    string getName(){
        return name;
    }

    long long power(long long base,long long e,long long mod){
        long long res=1;
        base=base%mod;
        while(e>0){
            if(e%2==1)
                res=(res*base)%mod;
            base=(base*base)%mod;
            e=e/2;
        }
        return res;
    }

    long long modInv(long long n,long long mod){
        for(long long x=1;x<mod;x++){
            if(((n%mod)*(x%mod))%mod==1)
                return x;
        }
        return -1;
    }

    long long gcd(long long a,long long b){
        while(b){
            a%=b;
            swap(a,b);
        }
        return a;
    }
    
    virtual string describeRole()=0;
    bool checkInput(const string &input,long long &result){
        bool validInput=true;
        bool isDecimal=false;
        if(input.empty()){
            return false;
        }
        for(char c:input){
            if(c=='.'){
                isDecimal = true;
                break;
            }
        }

        if(isDecimal){
            cout<<"Invalid input. Please enter a positive integer.\n";
            return false;
        }
        
        for(char c:input){
            if(!isdigit(c)){
                validInput=false;
                break;
            }
        }   

        if(!validInput){
            cout<<"Invalid input. Please enter a positive integer.\n";
            return false;
        }
        result=stoll(input);
        return true;
    }
};

class Signer: public Participant {
private:
    long long p,q,phiN,d;
    long long N=0,e;
public:
    long long getN(){ 
        return N;
    }
    long long getE(){ 
        return e;
    }

    void generateKey(){
        cout<<"Choose 2 distinct prime numbers (p and q):\n";
        while(true){
            string pStr,qStr;
            cin>>pStr>>qStr;
            long long PLL,QLL;
            if(!checkInput(pStr, PLL) || !checkInput(qStr, QLL)){
                continue;
            }
            p=PLL;
            q=QLL;
            if(p>3037000499LL || q>3037000499LL){
            cout<<"Numbers too large for safe calculation. Max allowed is 3037000499.\n";
            continue;
            }
            if(p<=1||q<=1){
                cout<<"Numbers must be greater than 1.\n";
                continue;
            }
            int found=0;
            for(long long i=2;i*i<=p;i++){
                if(p%i==0){cout<<"p is not Prime\n";
                found=1;
                break;
                }
            }
            for(long long i=2;i*i<=q;i++){
                if(q%i==0){cout<<"q is not Prime\n";
                found=1;
                break;
                }
            }
            if(found)
            continue;
            if(p==q){
                cout<<"Numbers must be distinct\n";
                continue;
            }
            break;
        }
        N=p*q;
        phiN=(p-1)*(q-1);
        cout<<"The value of N is "<<N;
        cout<<"\nValue of Euler's Totient phi(n) is "<<phiN;
        cout<<"\nChoose an integer e (public exponent) such that 1 < e < phi(n) and gcd(e, phi(n))=1\n";
        while(true){
            string eStr;
            cin>>eStr;
            long long ELL;
            if(!checkInput(eStr,ELL)){
                continue;
            }
            e=ELL;
            if(e>1&&e<phiN&&gcd(e,phiN)==1)
                break;
            cout<<"Invalid Entry for e\n";
        }
        d=modInv(e,phiN);
        cout<<"The value for d (private exponent) is "<<d;
        cout<<"\nThe public key is ("<<e<<","<<N<<") and the private key is ("<<d<<","<<N<<")";
    }

    long long signMessage(long long bMessage){
        return power(bMessage,d,N); 
    }

    string describeRole() override{
        return "The Signer's role is to generate keys and sign blinded messages.";
    }
};

class User: public Participant {
private:
    long long b;
    long long originalMessage;
public:
    long long blindMessage(long long M,long long e,long long N) {
        originalMessage=M;
        long long NLL=N;
        cout<<"Enter a secret blinding factor b:\n";
        while(true){
            string bStr;
            cin>>bStr;
            long long BLL;
            if(!checkInput(bStr,BLL)){
                continue;
            }
            b = BLL;
            if(gcd(b,NLL)==1) 
                break;
            cout<<"Invalid blinding factor. b must be coprime to N ("<<NLL<<").Try again.\n";
        }
        cout<<"\n";
        long long C=power(b,e,NLL);
        long long bM=(1LL*M*C)%NLL;
        return bM;
    }

    long long unblindMessage(long long bS,long long N){
        long long NLL=N;
        long long binv=modInv(b,NLL);
        return (1LL*bS*binv)%NLL;
    }

    void verifySign(long long signature,long long e,long long N){
        long long decryptedMessage=power(signature,e,N);
        cout<<"Message recovered from signature: "<<decryptedMessage<<"\n";
        if(decryptedMessage==originalMessage)
            cout<<"The signature is valid.\n";
        else
            cout<<"The signature is not valid.\n";
    }

    string describeRole() override{
        return "The User's role is to blind the message, get it signed, unblind it, and verify the signature.";
    }
};

int main(){
    string signerName,userName;
    cout<<"Enter signer name: ";
    getline(cin, signerName);
    cout<<"Enter user name: ";
    getline(cin, userName);

    Signer signer;
    signer.name=signerName;
    User user;
    user.name=userName;

    long long M=0;
    long long bMessage=-1;
    long long blindedSignature=-1;
    long long fSignature=-1;

    while(true){
        cout<<"\n\n\t\tMain Menu\n";
        cout<<"1. Generate Key\n";
        cout<<"2. Blind Message\n";
        cout<<"3. Sign Blinded Message \n";
        cout<<"4. Unblind Signature \n";
        cout<<"5. Verify Signature\n";
        cout<<"6. Explain roles\n";
        cout<<"7. Exit\n\n";
        cout<<"Enter your choice: ";
        string chStr;
        cin>>chStr;
        cout<<"\n";
        long long CLL;

        if (!signer.checkInput(chStr,CLL)){
            continue;
        }

        long long choice=CLL;
        if (choice<1 || choice>7) {
            cout << "Invalid choice. Please enter a number between 1 and 7.\n";
            continue;
        }

        if(choice==1){
            signer.generateKey();
            cout<<"\nKey Generated by "<<signer.getName()<<".\n";
        }else if(choice==2){
            if(signer.getN()==0){
                cout<<"Key hasn't been generated yet\n"; 
                continue;
            }
            string input;
            cout<<"Enter your message that you want to blind\n";
            while(true){
                cin>>input;
                long long MLL;
                if(!user.checkInput(input,MLL)){
                    continue;
                }
                M=MLL;
                if(M<0){
                    cout<<"Negative numbers are not allowed\n";
                    continue;
                }
                else if(M>signer.getN()){
                    cout<<"The message should be smaller than the value of N\n";
                    continue;
                }
                break;
            }
            bMessage=user.blindMessage(M,signer.getE(),signer.getN());
            cout<<"Message has been blinded by "<<user.getName()<<".\n";
            cout<<"The blinded message to send to the signer is: "<<bMessage<<"\n";
        }else if(choice==3){
            if(bMessage==-1){
                cout<<"There is no blinded message ready to sign\n"; 
                continue; 
            }
            blindedSignature=signer.signMessage(bMessage);
            cout<<"Blinded message has been signed by "<<signer.getName()<<".\n";
            cout<<"The blinded signature received is: "<<blindedSignature<<"\n";
        }else if(choice==4){
            if(blindedSignature==-1){ 
                cout<<"There is no blinded signature to unblind\n"; 
                continue; 
            }
            fSignature=user.unblindMessage(blindedSignature,signer.getN());
            cout<<"Signature has been unblinded by "<<user.getName()<<".\n";
            cout<<"The final, valid signature for the original message "<<M<<" is: "<<fSignature<<"\n";
        }else if(choice==5){
            if(fSignature==-1){
                cout<<"There is no final signature to verify\n"; 
                continue;
            }
            user.verifySign(fSignature,signer.getE(),signer.getN());
        }else if(choice==6){
            cout<<"\n";
            Participant* p1=&signer;
            Participant* p2=&user;
            cout<<"Participant 1 ("<<p1->getName()<<"): "<<p1->describeRole()<<endl;
            cout<<"Participant 2 ("<<p2->getName()<<"): "<<p2->describeRole()<<endl;
        }else if(choice==7){
            break;
        }
    }
    
    return 0;
}
