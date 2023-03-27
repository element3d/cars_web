#ifndef __ENUMS__
#define __ENUMS__

#include <string>
#include <vector>

class EMake
{
public:
   static std::string None;
   static std::string All;
   static std::string Bmw;
   static std::string MercedesBenz;
   static std::string Audi;
   static std::string Porsche;
   static std::string Lexus;
   static std::string Infiniti;
   static std::string Toyota;
   static std::string Nissan;
   static std::string Ford;
   static std::string Chevrolet;
   static std::string Volkswagen;
   static std::string Opel;
   static std::string Mitsubishi;
   static std::string Kia;
   static std::string Hyundai;
   static std::string Honda;
   static std::string Mazda;

   static std::string toString(const std::string& make)
    {
       if (EMake::Bmw == make) return "BMW";
       if (EMake::MercedesBenz == make) return "MERCEDES BENZ";
       if (EMake::Audi == make) return "AUDI";
       if (EMake::Porsche == make) return "PORSCHE";
       if (EMake::Lexus == make) return "LEXUS";
       if (EMake::Infiniti == make) return "INFINITI";
       if (EMake::Toyota == make) return "TOYOTA";
       if (EMake::Nissan == make) return "NISSAN";
       if (EMake::Ford == make) return "FORD";
       if (EMake::Chevrolet == make) return "CHEVROLET";
       if (EMake::Volkswagen == make) return "VOLKSWAGEN";
       if (EMake::Opel == make) return "OPEL";
       if (EMake::Mitsubishi == make) return "MITSUBISHI";
       if (EMake::Kia == make) return "KIA";
       if (EMake::Hyundai == make) return "HYUNDAI";
       if (EMake::Honda == make) return "HONDA";
       if (EMake::Mazda == make) return "MAZDA";
       return "";
   }

   static std::vector<std::string> ToArray()
    {
      std::vector<std::string> vec;
      vec.push_back(EMake::Bmw);
      vec.push_back(EMake::MercedesBenz);
      vec.push_back(EMake::Audi);
      vec.push_back(EMake::Porsche);
      vec.push_back(EMake::Lexus);
      vec.push_back(EMake::Infiniti);
      vec.push_back(EMake::Toyota);
      vec.push_back(EMake::Nissan);
      vec.push_back(EMake::Ford);
      vec.push_back(EMake::Chevrolet);
      vec.push_back(EMake::Volkswagen);
      vec.push_back(EMake::Opel);
      vec.push_back(EMake::Mitsubishi);
      vec.push_back(EMake::Kia);
      vec.push_back(EMake::Hyundai);
      vec.push_back(EMake::Honda);
      vec.push_back(EMake::Mazda);
      return vec;
    }
};

#endif
