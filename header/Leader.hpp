#ifndef LEADER_HPP
#define LEADER_HPP

#include "Entraineur.hpp"

class Leader : public Entraineur {
private:
    std::string gymnase;
    std::string badge;

public:
    Leader(std::string nom, std::string gymnase, std::string badge, const std::vector<std::string>& equipe);
    std::string getGymnase() const;
    std::string getBadge() const;
};

#endif
