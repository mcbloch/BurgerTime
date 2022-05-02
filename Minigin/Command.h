#pragma once

class Command
{
public:
	Command()          = default;
	virtual ~Command() = default;

	Command(const Command& other)                = delete;
	Command(Command&& other) noexcept            = delete;
	Command& operator=(const Command& other)     = delete;
	Command& operator=(Command&& other) noexcept = delete;

	virtual void Execute() = 0;
};


class MoveUpCommand final : public Command
{
public:
	void Execute() override;
};

class MoveRightCommand final : public Command
{
public:
	void Execute() override;
};

class MoveLeftCommand final : public Command
{
public:
	void Execute() override;
};

class MoveDownCommand final : public Command
{
public:
	void Execute() override;
};


class AnalogCommand
{
public:
	AnalogCommand()          = default;
	virtual ~AnalogCommand() = default;

	AnalogCommand(const AnalogCommand& other)                = delete;
	AnalogCommand(AnalogCommand&& other) noexcept            = delete;
	AnalogCommand& operator=(const AnalogCommand& other)     = delete;
	AnalogCommand& operator=(AnalogCommand&& other) noexcept = delete;

	virtual void Execute(short value) = 0;
};


// class BrakeCommand : public AnalogCommand
// {
// public:
// void Execute(short value) override;
// private:
// short lastValue = 0;
// };
